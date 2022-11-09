/****************************************************************************
**
** Copyright (C) 2008-2012 NVIDIA Corporation.
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick 3D.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtQuick3DRuntimeRender/private/qssgrenderer_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendererimpl_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderlayer_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendereffect_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderlight_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendercamera_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendercontextcore_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderresourcemanager_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderreferencedmaterial_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendereffectsystem_p.h>
#include <QtQuick3DRender/private/qssgrenderframebuffer_p.h>
#include <QtQuick3DRender/private/qssgrenderrenderbuffer_p.h>
#include <QtQuick3DRuntimeRender/private/qssgoffscreenrenderkey_p.h>
//#include <QtQuick3DRuntimeRender/private/qssgrenderplugin.h>
//#include <QtQuick3DRuntimeRender/private/qssgrenderplugingraphobject.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderresourcebufferobjects_p.h>
#include <QtQuick3DUtils/private/qssgperftimer_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderbuffermanager_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendercustommaterialsystem_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderrenderlist_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderpath_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrenderpathmanager_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendershadercache_p.h>
#include <QtQuick3DRuntimeRender/private/qssgperframeallocator_p.h>
#include <QtQuick3DUtils/private/qssgutils_p.h>

#ifdef _WIN32
#pragma warning(disable : 4355)
#endif

QT_BEGIN_NAMESPACE

namespace {

inline bool iSRenderObjectPtrLessThan(const QSSGRenderableObject *lhs, const QSSGRenderableObject *rhs)
{
    return lhs->cameraDistanceSq < rhs->cameraDistanceSq;
}
inline bool iSRenderObjectPtrGreatThan(const QSSGRenderableObject *lhs, const QSSGRenderableObject *rhs)
{
    return lhs->cameraDistanceSq > rhs->cameraDistanceSq;
}

void MaybeQueueNodeForRender(QSSGRenderNode &inNode,
                             QVector<QSSGRenderableNodeEntry> &outRenderables,
                             QVector<QSSGRenderCamera *> &outCameras,
                             QVector<QSSGRenderLight *> &outLights,
                             quint32 &ioDFSIndex)
{
    ++ioDFSIndex;
    inNode.dfsIndex = ioDFSIndex;
    if (inNode.isRenderableType())
        outRenderables.push_back(inNode);
    else if (inNode.type == QSSGRenderGraphObject::Type::Camera)
        outCameras.push_back(static_cast<QSSGRenderCamera *>(&inNode));
    else if (inNode.type == QSSGRenderGraphObject::Type::Light)
        outLights.push_back(static_cast<QSSGRenderLight *>(&inNode));

    for (QSSGRenderNode *theChild = inNode.firstChild; theChild != nullptr; theChild = theChild->nextSibling)
        MaybeQueueNodeForRender(*theChild, outRenderables, outCameras, outLights, ioDFSIndex);
}

bool HasValidLightProbe(QSSGRenderImage *inLightProbeImage)
{
    return inLightProbeImage && inLightProbeImage->m_textureData.m_texture;
}
}

QSSGDefaultMaterialPreparationResult::QSSGDefaultMaterialPreparationResult(QSSGShaderDefaultMaterialKey inKey)
    : firstImage(nullptr), opacity(1.0f), materialKey(inKey), dirty(false)
{
}

#define MAX_AA_LEVELS 8

QSSGLayerRenderPreparationData::QSSGLayerRenderPreparationData(QSSGRenderLayer &inLayer,
                                                                   const QSSGRef<QSSGRendererImpl> &inRenderer)
    : layer(inLayer)
    , renderer(inRenderer)
    , camera(nullptr)
    , featuresDirty(true)
    , featureSetHash(0)
    , tooManyLightsError(false)
{
}

QSSGLayerRenderPreparationData::~QSSGLayerRenderPreparationData() = default;

bool QSSGLayerRenderPreparationData::needsWidgetTexture() const
{
    return iRenderWidgets.size() > 0;
}

void QSSGLayerRenderPreparationData::setShaderFeature(const QByteArray &theStr, bool inValue)
{
    QSSGShaderPreprocessorFeature item(theStr, inValue);
    auto iter = features.cbegin();
    const auto end = features.cend();

    while (iter != end && iter->name != theStr)
        ++iter;

    if (iter != end) {
        if (iter->enabled != inValue) {
            iter->enabled = inValue;
            featuresDirty = true;
            featureSetHash = 0;
        }
    } else {
        features.push_back(item);
        featuresDirty = true;
        featureSetHash = 0;
    }
}

QVector<QSSGShaderPreprocessorFeature> QSSGLayerRenderPreparationData::getShaderFeatureSet()
{
    if (featuresDirty) {
        std::sort(features.begin(), features.end());
        featuresDirty = false;
    }
    return features;
}

size_t QSSGLayerRenderPreparationData::getShaderFeatureSetHash()
{
    if (!featureSetHash)
        featureSetHash = hashShaderFeatureSet(getShaderFeatureSet());
    return featureSetHash;
}

void QSSGLayerRenderPreparationData::createShadowMapManager()
{
    if (shadowMapManager)
        return;

    shadowMapManager = QSSGRenderShadowMap::create(renderer->demonContext());
}

bool QSSGLayerRenderPreparationData::usesOffscreenRenderer()
{
    if (lastFrameOffscreenRenderer)
        return true;

    //    if (m_Layer.m_RenderPlugin && m_Layer.m_RenderPlugin->m_Flags.IsActive()) {
    //        IRenderPluginInstance *theInstance =
    //                m_Renderer.GetDemonContext().GetRenderPluginManager().GetOrCreateRenderPluginInstance(
    //                    m_Layer.m_RenderPlugin->m_PluginPath, m_Layer.m_RenderPlugin);
    //        if (theInstance) {
    //            m_Renderer.GetDemonContext()
    //                    .GetOffscreenRenderManager()
    //                    .MaybeRegisterOffscreenRenderer(&theInstance, *theInstance);
    //            m_LastFrameOffscreenRenderer = theInstance;
    //        }
    //    }
    if (lastFrameOffscreenRenderer == nullptr)
        lastFrameOffscreenRenderer = renderer->demonContext()->offscreenRenderManager()->getOffscreenRenderer(layer.texturePath);
    return lastFrameOffscreenRenderer != nullptr;
}

QVector3D QSSGLayerRenderPreparationData::getCameraDirection()
{
    if (cameraDirection.hasValue() == false) {
        if (camera)
            cameraDirection = camera->getScalingCorrectDirection();
        else
            cameraDirection = QVector3D(0, 0, -1);
    }
    return *cameraDirection;
}

// Per-frame cache of renderable objects post-sort.
const QVector<QSSGRenderableObject *> &QSSGLayerRenderPreparationData::getOpaqueRenderableObjects()
{
    if (renderedOpaqueObjects.empty() == false || camera == nullptr)
        return renderedOpaqueObjects;
    if (layer.flags.testFlag(QSSGRenderLayer::Flag::LayerEnableDepthTest) && !opaqueObjects.empty()) {
        QVector3D theCameraDirection(getCameraDirection());
        QVector3D theCameraPosition = camera->getGlobalPos();
        renderedOpaqueObjects = opaqueObjects;
        // Setup the object's sorting information
        for (int idx = 0, end = renderedOpaqueObjects.size(); idx < end; ++idx) {
            QSSGRenderableObject &theInfo = *renderedOpaqueObjects[idx];
            QVector3D difference = theInfo.worldCenterPoint - theCameraPosition;
            theInfo.cameraDistanceSq = QVector3D::dotProduct(difference, theCameraDirection);
        }
        // Render nearest to furthest objects
        std::sort(renderedOpaqueObjects.begin(), renderedOpaqueObjects.end(), iSRenderObjectPtrLessThan);
    }
    return renderedOpaqueObjects;
}

// If layer depth test is false, this may also contain opaque objects.
const QVector<QSSGRenderableObject *> &QSSGLayerRenderPreparationData::getTransparentRenderableObjects()
{
    if (renderedTransparentObjects.empty() == false || camera == nullptr)
        return renderedTransparentObjects;

    renderedTransparentObjects = transparentObjects;

    if (!layer.flags.testFlag(QSSGRenderLayer::Flag::LayerEnableDepthTest))
        renderedTransparentObjects.append(opaqueObjects);

    if (renderedTransparentObjects.empty() == false) {
        QVector3D theCameraDirection(getCameraDirection());
        QVector3D theCameraPosition = camera->getGlobalPos();

        // Setup the object's sorting information
        for (quint32 idx = 0, end = renderedTransparentObjects.size(); idx < end; ++idx) {
            QSSGRenderableObject &theInfo = *renderedTransparentObjects[idx];
            QVector3D difference = theInfo.worldCenterPoint - theCameraPosition;
            theInfo.cameraDistanceSq = QVector3D::dotProduct(difference, theCameraDirection);
        }
        // render furthest to nearest.
        std::sort(renderedTransparentObjects.begin(), renderedTransparentObjects.end(), iSRenderObjectPtrGreatThan);
    }

    return renderedTransparentObjects;
}

#define MAX_LAYER_WIDGETS 200

void QSSGLayerRenderPreparationData::addRenderWidget(QSSGRenderWidgetInterface &inWidget)
{
    // The if the layer is not active then the widget can't be displayed.
    // Furthermore ResetForFrame won't be called below which leads to stale
    // widgets in the m_IRenderWidgets array.  These stale widgets would get rendered
    // the next time the layer was active potentially causing a crash.
    if (!layer.flags.testFlag(QSSGRenderLayer::Flag::Active))
        return;

    // Ensure we clear the widget layer always
    renderer->layerNeedsFrameClear(*static_cast<QSSGLayerRenderData *>(this));

    if (iRenderWidgets.size() < MAX_LAYER_WIDGETS)
        iRenderWidgets.push_back(&inWidget);
}

#define RENDER_FRAME_NEW(type) new (renderer->demonContext()->perFrameAllocator().allocate(sizeof(type))) type

#define QSSG_RENDER_MINIMUM_RENDER_OPACITY .01f

QSSGShaderDefaultMaterialKey QSSGLayerRenderPreparationData::generateLightingKey(QSSGRenderDefaultMaterial::MaterialLighting inLightingType, bool receivesShadows)
{
    QSSGShaderDefaultMaterialKey theGeneratedKey(getShaderFeatureSetHash());
    const bool lighting = inLightingType != QSSGRenderDefaultMaterial::MaterialLighting::NoLighting;
    renderer->defaultMaterialShaderKeyProperties().m_hasLighting.setValue(theGeneratedKey, lighting);
    if (lighting) {
        const bool lightProbe = layer.lightProbe && layer.lightProbe->m_textureData.m_texture;
        renderer->defaultMaterialShaderKeyProperties().m_hasIbl.setValue(theGeneratedKey, lightProbe);

        quint32 numLights = (quint32)globalLights.size();
        if (numLights > QSSGShaderDefaultMaterialKeyProperties::LightCount && tooManyLightsError == false) {
            tooManyLightsError = true;
            numLights = QSSGShaderDefaultMaterialKeyProperties::LightCount;
            qCCritical(INVALID_OPERATION, "Too many lights on layer, max is 7");
            Q_ASSERT(false);
        }
        renderer->defaultMaterialShaderKeyProperties().m_lightCount.setValue(theGeneratedKey, numLights);

        for (quint32 lightIdx = 0, lightEnd = globalLights.size(); lightIdx < lightEnd; ++lightIdx) {
            QSSGRenderLight *theLight(globalLights[lightIdx]);
            const bool isDirectional = theLight->m_lightType == QSSGRenderLight::Type::Directional;
            const bool isArea = theLight->m_lightType == QSSGRenderLight::Type::Area;
            const bool castShadowsArea = (theLight->m_lightType != QSSGRenderLight::Type::Area) && (theLight->m_castShadow) && receivesShadows;

            renderer->defaultMaterialShaderKeyProperties().m_lightFlags[lightIdx].setValue(theGeneratedKey, !isDirectional);
            renderer->defaultMaterialShaderKeyProperties().m_lightAreaFlags[lightIdx].setValue(theGeneratedKey, isArea);
            renderer->defaultMaterialShaderKeyProperties().m_lightShadowFlags[lightIdx].setValue(theGeneratedKey, castShadowsArea);
        }
    }
    return theGeneratedKey;
}

QPair<bool, QSSGRenderGraphObject *> QSSGLayerRenderPreparationData::resolveReferenceMaterial(QSSGRenderGraphObject *inMaterial)
{
    bool subsetDirty = false;
    bool badIdea = false;
    QSSGRenderGraphObject *theSourceMaterialObject(inMaterial);
    QSSGRenderGraphObject *theMaterialObject(inMaterial);
    while (theMaterialObject && theMaterialObject->type == QSSGRenderGraphObject::Type::ReferencedMaterial && !badIdea) {
        QSSGRenderReferencedMaterial *theRefMaterial = static_cast<QSSGRenderReferencedMaterial *>(theMaterialObject);
        theMaterialObject = theRefMaterial->m_referencedMaterial;
        if (theMaterialObject == theSourceMaterialObject) {
            badIdea = true;
        }

        if (theRefMaterial == theSourceMaterialObject) {
            theRefMaterial->m_dirty.updateDirtyForFrame();
        }
        subsetDirty = subsetDirty | theRefMaterial->m_dirty.isDirty();
    }
    if (badIdea) {
        theMaterialObject = nullptr;
    }
    return QPair<bool, QSSGRenderGraphObject *>(subsetDirty, theMaterialObject);
}

bool QSSGLayerRenderPreparationData::preparePathForRender(QSSGRenderPath &inPath,
                                                            const QMatrix4x4 &inViewProjection,
                                                            const QSSGOption<QSSGClippingFrustum> &inClipFrustum,
                                                            QSSGLayerRenderPreparationResultFlags &ioFlags)
{
    QSSGRenderableObjectFlags theSharedFlags;
    theSharedFlags.setPickable(true);
    float subsetOpacity = inPath.globalOpacity;
    bool retval = inPath.flags.testFlag(QSSGRenderPath::Flag::Dirty);
    inPath.flags.setFlag(QSSGRenderPath::Flag::Dirty, false);
    QMatrix4x4 theMVP;
    QMatrix3x3 theNormalMatrix;

    inPath.calculateMVPAndNormalMatrix(inViewProjection, theMVP, theNormalMatrix);
    QSSGBounds3 theBounds(this->renderer->demonContext()->pathManager()->getBounds(inPath));

    if (inPath.globalOpacity >= QSSG_RENDER_MINIMUM_RENDER_OPACITY && inClipFrustum.hasValue()) {
        // Check bounding box against the clipping planes
        QSSGBounds3 theGlobalBounds = theBounds;
        theGlobalBounds.transform(inPath.globalTransform);
        if (inClipFrustum->intersectsWith(theGlobalBounds) == false)
            subsetOpacity = 0.0f;
    }

    QSSGRenderGraphObject *theMaterials[2] = { inPath.m_material, inPath.m_secondMaterial };

    if (inPath.m_pathType == QSSGRenderPath::PathType::Geometry || inPath.m_paintStyle != QSSGRenderPath::PaintStyle::FilledAndStroked)
        theMaterials[1] = nullptr;

    // We need to fill material to be the first one rendered so the stroke goes on top.
    // In the timeline, however, this is reversed.

    if (theMaterials[1])
        std::swap(theMaterials[1], theMaterials[0]);

    for (quint32 idx = 0, end = 2; idx < end; ++idx) {
        if (theMaterials[idx] == nullptr)
            continue;

        QSSGRenderableObjectFlags theFlags = theSharedFlags;

        QPair<bool, QSSGRenderGraphObject *> theMaterialAndDirty(resolveReferenceMaterial(theMaterials[idx]));
        QSSGRenderGraphObject *theMaterial(theMaterialAndDirty.second);
        retval = retval || theMaterialAndDirty.first;

        if (theMaterial != nullptr && theMaterial->type == QSSGRenderGraphObject::Type::DefaultMaterial) {
            QSSGRenderDefaultMaterial *theDefaultMaterial = static_cast<QSSGRenderDefaultMaterial *>(theMaterial);
            // Don't clear dirty flags if the material was referenced.
            bool clearMaterialFlags = theMaterial == inPath.m_material;
            QSSGDefaultMaterialPreparationResult prepResult(
                    prepareDefaultMaterialForRender(*theDefaultMaterial, theFlags, subsetOpacity, clearMaterialFlags));

            theFlags = prepResult.renderableFlags;
            if (inPath.m_pathType == QSSGRenderPath::PathType::Geometry) {
                if ((inPath.m_beginCapping != QSSGRenderPath::Capping::None && inPath.m_beginCapOpacity < 1.0f)
                    || (inPath.m_endCapping != QSSGRenderPath::Capping::None && inPath.m_endCapOpacity < 1.0f))
                    theFlags.setHasTransparency(true);
            } else {
                ioFlags.setRequiresStencilBuffer(true);
            }
            retval = retval || prepResult.dirty;
            bool isStroke = true;
            if (idx == 0 && inPath.m_pathType == QSSGRenderPath::PathType::Painted) {
                if (inPath.m_paintStyle == QSSGRenderPath::PaintStyle::Filled || inPath.m_paintStyle == QSSGRenderPath::PaintStyle::FilledAndStroked)
                    isStroke = false;
            }

            QSSGPathRenderable *theRenderable = RENDER_FRAME_NEW(QSSGPathRenderable)(theFlags,
                                                                                         inPath.getGlobalPos(),
                                                                                         renderer,
                                                                                         inPath.globalTransform,
                                                                                         theBounds,
                                                                                         inPath,
                                                                                         theMVP,
                                                                                         theNormalMatrix,
                                                                                         *theMaterial,
                                                                                         prepResult.opacity,
                                                                                         prepResult.materialKey,
                                                                                         isStroke);
            theRenderable->m_firstImage = prepResult.firstImage;

            QSSGRef<QSSGRenderContextInterface> demonContext(renderer->demonContext());
            QSSGRef<QSSGPathManagerInterface> thePathManager = demonContext->pathManager();
            retval = thePathManager->prepareForRender(inPath) || retval;
            retval |= (inPath.m_wireframeMode != demonContext->wireframeMode());
            inPath.m_wireframeMode = demonContext->wireframeMode();

            if (theFlags.hasTransparency())
                transparentObjects.push_back(theRenderable);
            else
                opaqueObjects.push_back(theRenderable);
        } else if (theMaterial != nullptr && theMaterial->type == QSSGRenderGraphObject::Type::CustomMaterial) {
            QSSGRenderCustomMaterial *theCustomMaterial = static_cast<QSSGRenderCustomMaterial *>(theMaterial);
            // Don't clear dirty flags if the material was referenced.
            // bool clearMaterialFlags = theMaterial == inPath.m_Material;
            QSSGDefaultMaterialPreparationResult prepResult(prepareCustomMaterialForRender(*theCustomMaterial, theFlags, subsetOpacity));

            theFlags = prepResult.renderableFlags;
            if (inPath.m_pathType == QSSGRenderPath::PathType::Geometry) {
                if ((inPath.m_beginCapping != QSSGRenderPath::Capping::None && inPath.m_beginCapOpacity < 1.0f)
                    || (inPath.m_endCapping != QSSGRenderPath::Capping::None && inPath.m_endCapOpacity < 1.0f))
                    theFlags.setHasTransparency(true);
            } else {
                ioFlags.setRequiresStencilBuffer(true);
            }

            retval = retval || prepResult.dirty;
            bool isStroke = true;
            if (idx == 0 && inPath.m_pathType == QSSGRenderPath::PathType::Painted) {
                if (inPath.m_paintStyle == QSSGRenderPath::PaintStyle::Filled || inPath.m_paintStyle == QSSGRenderPath::PaintStyle::FilledAndStroked)
                    isStroke = false;
            }

            QSSGPathRenderable *theRenderable = RENDER_FRAME_NEW(QSSGPathRenderable)(theFlags,
                                                                                         inPath.getGlobalPos(),
                                                                                         renderer,
                                                                                         inPath.globalTransform,
                                                                                         theBounds,
                                                                                         inPath,
                                                                                         theMVP,
                                                                                         theNormalMatrix,
                                                                                         *theMaterial,
                                                                                         prepResult.opacity,
                                                                                         prepResult.materialKey,
                                                                                         isStroke);
            theRenderable->m_firstImage = prepResult.firstImage;

            QSSGRef<QSSGRenderContextInterface> demonContext(renderer->demonContext());
            QSSGRef<QSSGPathManagerInterface> thePathManager = demonContext->pathManager();
            retval = thePathManager->prepareForRender(inPath) || retval;
            retval |= (inPath.m_wireframeMode != demonContext->wireframeMode());
            inPath.m_wireframeMode = demonContext->wireframeMode();

            if (theFlags.hasTransparency())
                transparentObjects.push_back(theRenderable);
            else
                opaqueObjects.push_back(theRenderable);
        }
    }
    return retval;
}

void QSSGLayerRenderPreparationData::prepareImageForRender(QSSGRenderImage &inImage,
                                                             QSSGImageMapTypes inMapType,
                                                             QSSGRenderableImage *&ioFirstImage,
                                                             QSSGRenderableImage *&ioNextImage,
                                                             QSSGRenderableObjectFlags &ioFlags,
                                                             QSSGShaderDefaultMaterialKey &inShaderKey,
                                                             quint32 inImageIndex)
{
    const QSSGRef<QSSGRenderContextInterface> &demonContext(renderer->demonContext());
    const QSSGRef<QSSGBufferManager> &bufferManager = demonContext->bufferManager();
    const QSSGRef<QSSGOffscreenRenderManager> &theOffscreenRenderManager(demonContext->offscreenRenderManager());
    //    IRenderPluginManager &theRenderPluginManager(demonContext.GetRenderPluginManager());
    if (inImage.clearDirty(bufferManager, *theOffscreenRenderManager /*, theRenderPluginManager*/))
        ioFlags |= QSSGRenderableObjectFlag::Dirty;

    // All objects with offscreen renderers are pickable so we can pass the pick through to the
    // offscreen renderer and let it deal with the pick.
    if (inImage.m_lastFrameOffscreenRenderer != nullptr) {
        ioFlags.setPickable(true);
        ioFlags |= QSSGRenderableObjectFlag::HasTransparency;
    }

    if (inImage.m_textureData.m_texture) {
        if (inImage.m_textureData.m_textureFlags.hasTransparency()
            && (inMapType == QSSGImageMapTypes::Diffuse || inMapType == QSSGImageMapTypes::Opacity
                || inMapType == QSSGImageMapTypes::Translucency)) {
            ioFlags |= QSSGRenderableObjectFlag::HasTransparency;
        }
        // Textures used in general have linear characteristics.
        // PKC -- The filters are properly set already.  Setting them here only overrides what
        // would
        // otherwise be a correct setting.
        // inImage.m_TextureData.m_Texture->SetMinFilter( QSSGRenderTextureMinifyingOp::Linear );
        // inImage.m_TextureData.m_Texture->SetMagFilter( QSSGRenderTextureMagnifyingOp::Linear );

        QSSGRenderableImage *theImage = RENDER_FRAME_NEW(QSSGRenderableImage)(inMapType, inImage);
        QSSGShaderKeyImageMap &theKeyProp = renderer->defaultMaterialShaderKeyProperties().m_imageMaps[inImageIndex];

        theKeyProp.setEnabled(inShaderKey, true);
        switch (inImage.m_mappingMode) {
        default:
            Q_ASSERT(false);
            // fallthrough intentional
        case QSSGRenderImage::MappingModes::Normal:
            break;
        case QSSGRenderImage::MappingModes::Environment:
            theKeyProp.setEnvMap(inShaderKey, true);
            break;
        case QSSGRenderImage::MappingModes::LightProbe:
            theKeyProp.setLightProbe(inShaderKey, true);
            break;
        }

        if (inImage.m_textureData.m_textureFlags.isInvertUVCoords())
            theKeyProp.setInvertUVMap(inShaderKey, true);
        if (ioFirstImage == nullptr)
            ioFirstImage = theImage;
        else
            ioNextImage->m_nextImage = theImage;

        // assume offscreen renderer produces non-premultiplied image
        if (inImage.m_lastFrameOffscreenRenderer == nullptr && inImage.m_textureData.m_textureFlags.isPreMultiplied())
            theKeyProp.setPremultiplied(inShaderKey, true);

        QSSGShaderKeyTextureSwizzle &theSwizzleKeyProp = renderer->defaultMaterialShaderKeyProperties().m_textureSwizzle[inImageIndex];
        theSwizzleKeyProp.setSwizzleMode(inShaderKey, inImage.m_textureData.m_texture->textureSwizzleMode(), true);

        ioNextImage = theImage;
    }
}

QSSGDefaultMaterialPreparationResult QSSGLayerRenderPreparationData::prepareDefaultMaterialForRender(
        QSSGRenderDefaultMaterial &inMaterial,
        QSSGRenderableObjectFlags &inExistingFlags,
        float inOpacity,
        bool inClearDirtyFlags)
{
    QSSGRenderDefaultMaterial *theMaterial = &inMaterial;
    QSSGDefaultMaterialPreparationResult retval(generateLightingKey(theMaterial->lighting, inExistingFlags.receivesShadows()));
    retval.renderableFlags = inExistingFlags;
    QSSGRenderableObjectFlags &renderableFlags(retval.renderableFlags);
    QSSGShaderDefaultMaterialKey &theGeneratedKey(retval.materialKey);
    retval.opacity = inOpacity;
    float &subsetOpacity(retval.opacity);

    if (theMaterial->dirty.isDirty()) {
        renderableFlags |= QSSGRenderableObjectFlag::Dirty;
    }
    subsetOpacity *= theMaterial->opacity;
    if (inClearDirtyFlags)
        theMaterial->dirty.updateDirtyForFrame();

    QSSGRenderableImage *firstImage = nullptr;

    // set wireframe mode
    renderer->defaultMaterialShaderKeyProperties().m_wireframeMode.setValue(theGeneratedKey,
                                                                            renderer->demonContext()->wireframeMode());

    if (theMaterial->iblProbe && checkLightProbeDirty(*theMaterial->iblProbe)) {
        renderer->prepareImageForIbl(*theMaterial->iblProbe);
    }

    if (!renderer->defaultMaterialShaderKeyProperties().m_hasIbl.getValue(theGeneratedKey)) {
        bool lightProbeValid = HasValidLightProbe(theMaterial->iblProbe);
        setShaderFeature(QSSGShaderDefines::lightProbe(), lightProbeValid);
        renderer->defaultMaterialShaderKeyProperties().m_hasIbl.setValue(theGeneratedKey, lightProbeValid);
        // setShaderFeature(ShaderFeatureDefines::enableIblFov(),
        // m_Renderer.GetLayerRenderData()->m_Layer.m_ProbeFov < 180.0f );
    }

    if (subsetOpacity >= QSSG_RENDER_MINIMUM_RENDER_OPACITY) {

        if (theMaterial->blendMode != QSSGRenderDefaultMaterial::MaterialBlendMode::Normal || theMaterial->opacityMap) {
            renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;
        }

        bool specularEnabled = theMaterial->isSpecularEnabled();
        renderer->defaultMaterialShaderKeyProperties().m_specularEnabled.setValue(theGeneratedKey, specularEnabled);
        if (specularEnabled) {
            renderer->defaultMaterialShaderKeyProperties().m_specularModel.setSpecularModel(theGeneratedKey, theMaterial->specularModel);
        }

        renderer->defaultMaterialShaderKeyProperties().m_fresnelEnabled.setValue(theGeneratedKey, theMaterial->isFresnelEnabled());

        renderer->defaultMaterialShaderKeyProperties().m_vertexColorsEnabled.setValue(theGeneratedKey,
                                                                                      theMaterial->isVertexColorsEnabled());

        // Run through the material's images and prepare them for render.
        // this may in fact set pickable on the renderable flags if one of the images
        // links to a sub presentation or any offscreen rendered object.
        QSSGRenderableImage *nextImage = nullptr;
#define CHECK_IMAGE_AND_PREPARE(img, imgtype, shadercomponent)                                                         \
    if ((img))                                                                                                         \
        prepareImageForRender(*(img), imgtype, firstImage, nextImage, renderableFlags, theGeneratedKey, shadercomponent);

        CHECK_IMAGE_AND_PREPARE(theMaterial->diffuseMaps[0],
                                QSSGImageMapTypes::Diffuse,
                                QSSGShaderDefaultMaterialKeyProperties::DiffuseMap0);
        CHECK_IMAGE_AND_PREPARE(theMaterial->diffuseMaps[1],
                                QSSGImageMapTypes::Diffuse,
                                QSSGShaderDefaultMaterialKeyProperties::DiffuseMap1);
        CHECK_IMAGE_AND_PREPARE(theMaterial->diffuseMaps[2],
                                QSSGImageMapTypes::Diffuse,
                                QSSGShaderDefaultMaterialKeyProperties::DiffuseMap2);
        CHECK_IMAGE_AND_PREPARE(theMaterial->emissiveMap, QSSGImageMapTypes::Emissive, QSSGShaderDefaultMaterialKeyProperties::EmissiveMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->emissiveMap2, QSSGImageMapTypes::Emissive, QSSGShaderDefaultMaterialKeyProperties::EmissiveMap2);
        CHECK_IMAGE_AND_PREPARE(theMaterial->specularReflection,
                                QSSGImageMapTypes::Specular,
                                QSSGShaderDefaultMaterialKeyProperties::SpecularMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->roughnessMap,
                                QSSGImageMapTypes::Roughness,
                                QSSGShaderDefaultMaterialKeyProperties::RoughnessMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->opacityMap, QSSGImageMapTypes::Opacity, QSSGShaderDefaultMaterialKeyProperties::OpacityMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->bumpMap, QSSGImageMapTypes::Bump, QSSGShaderDefaultMaterialKeyProperties::BumpMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->specularMap,
                                QSSGImageMapTypes::SpecularAmountMap,
                                QSSGShaderDefaultMaterialKeyProperties::SpecularAmountMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->normalMap, QSSGImageMapTypes::Normal, QSSGShaderDefaultMaterialKeyProperties::NormalMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->displacementMap,
                                QSSGImageMapTypes::Displacement,
                                QSSGShaderDefaultMaterialKeyProperties::DisplacementMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->translucencyMap,
                                QSSGImageMapTypes::Translucency,
                                QSSGShaderDefaultMaterialKeyProperties::TranslucencyMap);
        CHECK_IMAGE_AND_PREPARE(theMaterial->lightmaps.m_lightmapIndirect,
                                QSSGImageMapTypes::LightmapIndirect,
                                QSSGShaderDefaultMaterialKeyProperties::LightmapIndirect);
        CHECK_IMAGE_AND_PREPARE(theMaterial->lightmaps.m_lightmapRadiosity,
                                QSSGImageMapTypes::LightmapRadiosity,
                                QSSGShaderDefaultMaterialKeyProperties::LightmapRadiosity);
        CHECK_IMAGE_AND_PREPARE(theMaterial->lightmaps.m_lightmapShadow,
                                QSSGImageMapTypes::LightmapShadow,
                                QSSGShaderDefaultMaterialKeyProperties::LightmapShadow);
    }
#undef CHECK_IMAGE_AND_PREPARE

    if (subsetOpacity < QSSG_RENDER_MINIMUM_RENDER_OPACITY) {
        subsetOpacity = 0.0f;
        // You can still pick against completely transparent objects(or rather their bounding
        // box)
        // you just don't render them.
        renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;
        renderableFlags |= QSSGRenderableObjectFlag::CompletelyTransparent;
    }

    if (subsetOpacity > 1.f - QSSG_RENDER_MINIMUM_RENDER_OPACITY)
        subsetOpacity = 1.f;
    else
        renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;

    retval.firstImage = firstImage;
    if (retval.renderableFlags.isDirty())
        retval.dirty = true;
    return retval;
}

QSSGDefaultMaterialPreparationResult QSSGLayerRenderPreparationData::prepareCustomMaterialForRender(QSSGRenderCustomMaterial &inMaterial,
                                                                                                        QSSGRenderableObjectFlags &inExistingFlags,
                                                                                                        float inOpacity)
{
    QSSGDefaultMaterialPreparationResult retval(generateLightingKey(QSSGRenderDefaultMaterial::MaterialLighting::FragmentLighting, inExistingFlags.receivesShadows())); // always fragment lighting
    retval.renderableFlags = inExistingFlags;
    QSSGRenderableObjectFlags &renderableFlags(retval.renderableFlags);
    QSSGShaderDefaultMaterialKey &theGeneratedKey(retval.materialKey);
    retval.opacity = inOpacity;
    float &subsetOpacity(retval.opacity);

    // set wireframe mode
    renderer->defaultMaterialShaderKeyProperties().m_wireframeMode.setValue(theGeneratedKey,
                                                                            renderer->demonContext()->wireframeMode());

    if (subsetOpacity < QSSG_RENDER_MINIMUM_RENDER_OPACITY) {
        subsetOpacity = 0.0f;
        // You can still pick against completely transparent objects(or rather their bounding
        // box)
        // you just don't render them.
        renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;
        renderableFlags |= QSSGRenderableObjectFlag::CompletelyTransparent;
    }

    if (subsetOpacity > 1.f - QSSG_RENDER_MINIMUM_RENDER_OPACITY)
        subsetOpacity = 1.f;
    else
        renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;

    QSSGRenderableImage *firstImage = nullptr;
    QSSGRenderableImage *nextImage = nullptr;

#define CHECK_IMAGE_AND_PREPARE(img, imgtype, shadercomponent)                                                         \
    if ((img))                                                                                                         \
        prepareImageForRender(*(img), imgtype, firstImage, nextImage, renderableFlags, theGeneratedKey, shadercomponent);

    CHECK_IMAGE_AND_PREPARE(inMaterial.m_displacementMap,
                            QSSGImageMapTypes::Displacement,
                            QSSGShaderDefaultMaterialKeyProperties::DisplacementMap);
    CHECK_IMAGE_AND_PREPARE(inMaterial.m_lightmaps.m_lightmapIndirect,
                            QSSGImageMapTypes::LightmapIndirect,
                            QSSGShaderDefaultMaterialKeyProperties::LightmapIndirect);
    CHECK_IMAGE_AND_PREPARE(inMaterial.m_lightmaps.m_lightmapRadiosity,
                            QSSGImageMapTypes::LightmapRadiosity,
                            QSSGShaderDefaultMaterialKeyProperties::LightmapRadiosity);
    CHECK_IMAGE_AND_PREPARE(inMaterial.m_lightmaps.m_lightmapShadow,
                            QSSGImageMapTypes::LightmapShadow,
                            QSSGShaderDefaultMaterialKeyProperties::LightmapShadow);
#undef CHECK_IMAGE_AND_PREPARE

    retval.firstImage = firstImage;
    return retval;
}

bool QSSGLayerRenderPreparationData::prepareModelForRender(QSSGRenderModel &inModel,
                                                             const QMatrix4x4 &inViewProjection,
                                                             const QSSGOption<QSSGClippingFrustum> &inClipFrustum,
                                                             QSSGNodeLightEntryList &inScopedLights)
{
    const QSSGRef<QSSGRenderContextInterface> &demonContext(renderer->demonContext());
    const QSSGRef<QSSGBufferManager> &bufferManager = demonContext->bufferManager();
    QSSGRenderMesh *theMesh = bufferManager->loadMesh(inModel.meshPath);
    if (theMesh == nullptr)
        return false;

    QSSGModelContext &theModelContext = *RENDER_FRAME_NEW(QSSGModelContext)(inModel, inViewProjection);
    modelContexts.push_back(&theModelContext);

    bool subsetDirty = false;

    const QSSGScopedLightsListScope lightsScope(globalLights, lightDirections, sourceLightDirections, inScopedLights);
    setShaderFeature(QSSGShaderDefines::cgLighting(), !globalLights.empty());
    for (int idx = 0; idx < theMesh->subsets.size(); ++idx) {
        // If the materials list < size of subsets, then use the last material for the rest
        QSSGRenderGraphObject *theSourceMaterialObject = nullptr;
        if (inModel.materials.isEmpty())
            break;
        if (idx + 1 > inModel.materials.count())
            theSourceMaterialObject = inModel.materials.last();
        else
            theSourceMaterialObject = inModel.materials.at(idx);
        QSSGRenderSubset &theOuterSubset(theMesh->subsets[idx]);
        {
            QSSGRenderSubset &theSubset(theOuterSubset);
            QSSGRenderableObjectFlags renderableFlags;
            renderableFlags.setPickable(false);
            float subsetOpacity = inModel.globalOpacity;
            QVector3D theModelCenter(theSubset.bounds.center());
            theModelCenter = mat44::transform(inModel.globalTransform, theModelCenter);

            if (subsetOpacity >= QSSG_RENDER_MINIMUM_RENDER_OPACITY && inClipFrustum.hasValue()) {
                // Check bounding box against the clipping planes
                QSSGBounds3 theGlobalBounds = theSubset.bounds;
                theGlobalBounds.transform(theModelContext.model.globalTransform);
                if (inClipFrustum->intersectsWith(theGlobalBounds) == false)
                    subsetOpacity = 0.0f;
            }

            // For now everything is pickable.  Eventually we want to have localPickable and
            // globalPickable set on the node during
            // updates and have the runtime tell us what is pickable and what is not pickable.
            // Completely transparent models cannot be pickable.  But models with completely
            // transparent materials
            // still are.  This allows the artist to control pickability in a somewhat
            // fine-grained style.
            bool canModelBePickable = inModel.globalOpacity > .01f;
            renderableFlags.setPickable(canModelBePickable
                                        && (theModelContext.model.flags.testFlag(QSSGRenderModel::Flag::GloballyPickable)
                                            || renderableFlags.isPickable()));

            // Casting and Receiving Shadows
            renderableFlags.setCastsShadows(inModel.castsShadows);
            renderableFlags.setReceivesShadows(inModel.receivesShadows);

            QSSGRenderableObject *theRenderableObject = nullptr;
            QPair<bool, QSSGRenderGraphObject *> theMaterialObjectAndDirty = resolveReferenceMaterial(theSourceMaterialObject);
            QSSGRenderGraphObject *theMaterialObject = theMaterialObjectAndDirty.second;
            subsetDirty = subsetDirty || theMaterialObjectAndDirty.first;
            if (theMaterialObject == nullptr)
                continue;

            // set tessellation
            if (inModel.tessellationMode != TessModeValues::NoTess) {
                theSubset.primitiveType = QSSGRenderDrawMode::Patches;
                // set tessellation factor
                theSubset.edgeTessFactor = inModel.edgeTess;
                theSubset.innerTessFactor = inModel.innerTess;
                // update the vertex ver patch count in the input assembler
                // currently we only support triangle patches so count is always 3
                theSubset.inputAssembler->setPatchVertexCount(3);
                theSubset.inputAssemblerDepth->setPatchVertexCount(3);
                // check wireframe mode
                theSubset.wireframeMode = demonContext->wireframeMode();

                subsetDirty = subsetDirty | (theSubset.wireframeMode != inModel.wireframeMode);
                inModel.wireframeMode = demonContext->wireframeMode();
            } else {
                theSubset.primitiveType = theSubset.inputAssembler->drawMode();
                theSubset.inputAssembler->setPatchVertexCount(1);
                theSubset.inputAssemblerDepth->setPatchVertexCount(1);
                // currently we allow wirframe mode only if tessellation is on
                theSubset.wireframeMode = false;

                subsetDirty = subsetDirty | (theSubset.wireframeMode != inModel.wireframeMode);
                inModel.wireframeMode = false;
            }
            // Only clear flags on the materials in this direct hierarchy.  Do not clear them of
            // this
            // references materials in another hierarchy.
            bool clearMaterialDirtyFlags = theMaterialObject == theSourceMaterialObject;

            if (theMaterialObject == nullptr)
                continue;

            if (theMaterialObject->type == QSSGRenderGraphObject::Type::DefaultMaterial) {
                QSSGRenderDefaultMaterial &theMaterial(static_cast<QSSGRenderDefaultMaterial &>(*theMaterialObject));
                QSSGDefaultMaterialPreparationResult theMaterialPrepResult(
                        prepareDefaultMaterialForRender(theMaterial, renderableFlags, subsetOpacity, clearMaterialDirtyFlags));
                QSSGShaderDefaultMaterialKey theGeneratedKey = theMaterialPrepResult.materialKey;
                subsetOpacity = theMaterialPrepResult.opacity;
                QSSGRenderableImage *firstImage(theMaterialPrepResult.firstImage);
                subsetDirty |= theMaterialPrepResult.dirty;
                renderableFlags = theMaterialPrepResult.renderableFlags;

                renderer->defaultMaterialShaderKeyProperties().m_tessellationMode.setTessellationMode(theGeneratedKey,
                                                                                                      inModel.tessellationMode,
                                                                                                      true);

                QSSGDataView<QMatrix4x4> boneGlobals;
                if (theSubset.joints.size()) {
                    Q_ASSERT(false);
                }

                theRenderableObject = RENDER_FRAME_NEW(QSSGSubsetRenderable)(renderableFlags,
                                                                               theModelCenter,
                                                                               renderer,
                                                                               theSubset,
                                                                               theMaterial,
                                                                               theModelContext,
                                                                               subsetOpacity,
                                                                               firstImage,
                                                                               theGeneratedKey,
                                                                               boneGlobals);
                subsetDirty = subsetDirty || renderableFlags.isDirty();
            } else if (theMaterialObject->type == QSSGRenderGraphObject::Type::CustomMaterial) {
                QSSGRenderCustomMaterial &theMaterial(static_cast<QSSGRenderCustomMaterial &>(*theMaterialObject));

                const QSSGRef<QSSGMaterialSystem> &theMaterialSystem(demonContext->customMaterialSystem());
                subsetDirty |= theMaterialSystem->prepareForRender(theModelContext.model, theSubset, theMaterial, clearMaterialDirtyFlags);

                QSSGDefaultMaterialPreparationResult theMaterialPrepResult(
                        prepareCustomMaterialForRender(theMaterial, renderableFlags, subsetOpacity));
                QSSGShaderDefaultMaterialKey theGeneratedKey = theMaterialPrepResult.materialKey;
                subsetOpacity = theMaterialPrepResult.opacity;
                QSSGRenderableImage *firstImage(theMaterialPrepResult.firstImage);
                renderableFlags = theMaterialPrepResult.renderableFlags;

                // prepare for render tells us if the object is transparent
                if (theMaterial.m_hasTransparency)
                    renderableFlags |= QSSGRenderableObjectFlag::HasTransparency;
                // prepare for render tells us if the object is transparent
                if (theMaterial.m_hasRefraction)
                    renderableFlags |= QSSGRenderableObjectFlag::HasRefraction;

                renderer->defaultMaterialShaderKeyProperties().m_tessellationMode.setTessellationMode(theGeneratedKey,
                                                                                                      inModel.tessellationMode,
                                                                                                      true);

                if (theMaterial.m_iblProbe && checkLightProbeDirty(*theMaterial.m_iblProbe)) {
                    renderer->prepareImageForIbl(*theMaterial.m_iblProbe);
                }

                theRenderableObject = RENDER_FRAME_NEW(QSSGCustomMaterialRenderable)(renderableFlags,
                                                                                       theModelCenter,
                                                                                       renderer,
                                                                                       theSubset,
                                                                                       theMaterial,
                                                                                       theModelContext,
                                                                                       subsetOpacity,
                                                                                       firstImage,
                                                                                       theGeneratedKey);
            }
            if (theRenderableObject) {
                theRenderableObject->scopedLights = inScopedLights;
                // set tessellation
                theRenderableObject->tessellationMode = inModel.tessellationMode;

                if (theRenderableObject->renderableFlags.hasTransparency() || theRenderableObject->renderableFlags.hasRefraction()) {
                    transparentObjects.push_back(theRenderableObject);
                } else {
                    opaqueObjects.push_back(theRenderableObject);
                }
            }
        }
    }
    return subsetDirty;
}

bool QSSGLayerRenderPreparationData::prepareRenderablesForRender(const QMatrix4x4 &inViewProjection,
                                                                   const QSSGOption<QSSGClippingFrustum> &inClipFrustum,
                                                                   QSSGLayerRenderPreparationResultFlags &ioFlags)
{
    QSSGStackPerfTimer perfTimer(renderer->demonContext()->performanceTimer(), Q_FUNC_INFO);
    viewProjection = inViewProjection;
    bool wasDataDirty = false;
    for (qint32 idx = 0, end = renderableNodes.size(); idx < end; ++idx) {
        QSSGRenderableNodeEntry &theNodeEntry(renderableNodes[idx]);
        QSSGRenderNode *theNode = theNodeEntry.node;
        wasDataDirty = wasDataDirty || theNode->flags.testFlag(QSSGRenderNode::Flag::Dirty);
        switch (theNode->type) {
        case QSSGRenderGraphObject::Type::Model: {
            QSSGRenderModel *theModel = static_cast<QSSGRenderModel *>(theNode);
            theModel->calculateGlobalVariables();
            if (theModel->flags.testFlag(QSSGRenderModel::Flag::GloballyActive)) {
                bool wasModelDirty = prepareModelForRender(*theModel, inViewProjection, inClipFrustum, theNodeEntry.lights);
                wasDataDirty = wasDataDirty || wasModelDirty;
            }
        } break;
        case QSSGRenderGraphObject::Type::Path: {
            QSSGRenderPath *thePath = static_cast<QSSGRenderPath *>(theNode);
            thePath->calculateGlobalVariables();
            if (thePath->flags.testFlag(QSSGRenderPath::Flag::GloballyActive)) {
                bool wasPathDirty = preparePathForRender(*thePath, inViewProjection, inClipFrustum, ioFlags);
                wasDataDirty = wasDataDirty || wasPathDirty;
            }
        } break;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    return wasDataDirty;
}

bool QSSGLayerRenderPreparationData::checkLightProbeDirty(QSSGRenderImage &inLightProbe)
{
    const QSSGRef<QSSGRenderContextInterface> &theContext(renderer->demonContext());
    const QSSGRef<QSSGBufferManager> &bufferManager = theContext->bufferManager();
    return inLightProbe.clearDirty(bufferManager,
                                   *theContext->offscreenRenderManager() /*,
                                    theContext.GetRenderPluginManager()*/
                                   ,
                                   true);
}

struct QSSGLightNodeMarker
{
    QSSGRenderLight *light = nullptr;
    quint32 lightIndex = 0;
    quint32 firstValidIndex = 0;
    quint32 justPastLastValidIndex = 0;
    bool addOrRemove = false;
    QSSGLightNodeMarker() = default;
    QSSGLightNodeMarker(QSSGRenderLight &inLight, quint32 inLightIndex, QSSGRenderNode &inNode, bool aorm)
        : light(&inLight), lightIndex(inLightIndex), addOrRemove(aorm)
    {
        if (inNode.type == QSSGRenderGraphObject::Type::Layer) {
            firstValidIndex = 0;
            justPastLastValidIndex = std::numeric_limits<quint32>::max();
        } else {
            firstValidIndex = inNode.dfsIndex;
            QSSGRenderNode *lastChild = nullptr;
            QSSGRenderNode *firstChild = inNode.firstChild;
            // find deepest last child
            while (firstChild) {
                for (QSSGRenderNode *childNode = firstChild; childNode; childNode = childNode->nextSibling)
                    lastChild = childNode;

                if (lastChild)
                    firstChild = lastChild->firstChild;
                else
                    firstChild = nullptr;
            }
            if (lastChild)
                // last valid index would be the last child index + 1
                justPastLastValidIndex = lastChild->dfsIndex + 1;
            else // no children.
                justPastLastValidIndex = firstValidIndex + 1;
        }
    }
};

// m_Layer.m_Camera->CalculateViewProjectionMatrix(m_ViewProjection);
void QSSGLayerRenderPreparationData::prepareForRender(const QSize &inViewportDimensions, bool forceDirectRender)
{
    QSSGStackPerfTimer perfTimer(renderer->demonContext()->performanceTimer(), Q_FUNC_INFO);
    if (layerPrepResult.hasValue())
        return;

    features.clear();
    featureSetHash = 0;
    QVector2D thePresentationDimensions((float)inViewportDimensions.width(), (float)inViewportDimensions.height());
    const QSSGRef<QSSGRenderList> &theGraph(renderer->demonContext()->renderList());
    QRect theViewport(theGraph->getViewport());
    QRect theScissor(theGraph->getViewport());
    if (theGraph->isScissorTestEnabled())
        theScissor = renderer->context()->scissorRect();
    bool wasDirty = false;
    bool wasDataDirty = false;
    wasDirty = layer.flags.testFlag(QSSGRenderLayer::Flag::Dirty);
    // The first pass is just to render the data.
    quint32 maxNumAAPasses = layer.progressiveAAMode == QSSGRenderLayer::AAMode::NoAA ? (quint32)0 : (quint32)(layer.progressiveAAMode) + 1;
    maxNumAAPasses = qMin((quint32)(MAX_AA_LEVELS + 1), maxNumAAPasses);
    QSSGRenderEffect *theLastEffect = nullptr;
    // Uncomment the line below to disable all progressive AA.
    // maxNumAAPasses = 0;

    QSSGLayerRenderPreparationResult thePrepResult;
    bool hasOffscreenRenderer = usesOffscreenRenderer();

    bool SSAOEnabled = (layer.aoStrength > 0.0f && layer.aoDistance > 0.0f);
    bool SSDOEnabled = (layer.shadowStrength > 0.0f && layer.shadowDist > 0.0f);
    setShaderFeature(QSSGShaderDefines::ssao(), SSAOEnabled);
    setShaderFeature(QSSGShaderDefines::ssdo(), SSDOEnabled);
    bool requiresDepthPrepass = (hasOffscreenRenderer == false) && (SSAOEnabled || SSDOEnabled);
    setShaderFeature(QSSGShaderDefines::ssm(), false); // by default no shadow map generation

    if (layer.flags.testFlag(QSSGRenderLayer::Flag::Active)) {
        // Get the layer's width and height.
        const QSSGRef<QSSGEffectSystem> &theEffectSystem(renderer->demonContext()->effectSystem());
        for (QSSGRenderEffect *theEffect = layer.firstEffect; theEffect; theEffect = theEffect->m_nextEffect) {
            if (theEffect->flags.testFlag(QSSGRenderEffect::Flag::Dirty)) {
                wasDirty = true;
                theEffect->flags.setFlag(QSSGRenderEffect::Flag::Dirty, false);
            }
            if (theEffect->flags.testFlag(QSSGRenderEffect::Flag::Active)) {
                theLastEffect = theEffect;
                if (hasOffscreenRenderer == false && theEffectSystem->doesEffectRequireDepthTexture(theEffect->className))
                    requiresDepthPrepass = true;
            }
        }
        if (layer.flags.testFlag(QSSGRenderLayer::Flag::Dirty)) {
            wasDirty = true;
            layer.calculateGlobalVariables();
        }

        bool shouldRenderToTexture = true;

        if (hasOffscreenRenderer || forceDirectRender) {
            // We don't render to texture with offscreen renderers, we just render them to the
            // viewport.
            shouldRenderToTexture = false;
            // Progaa disabled when using offscreen rendering.
            maxNumAAPasses = 0;
        }

        thePrepResult = QSSGLayerRenderPreparationResult(
                QSSGLayerRenderHelper(theViewport,
                                        theScissor,
                                        layer,
                                        shouldRenderToTexture,
                                        renderer->demonContext()->scaleMode(),
                                        renderer->demonContext()->presentationScaleFactor()));
        thePrepResult.lastEffect = theLastEffect;
        thePrepResult.maxAAPassIndex = maxNumAAPasses;
        thePrepResult.flags.setRequiresDepthTexture(requiresDepthPrepass || needsWidgetTexture());
        thePrepResult.flags.setShouldRenderToTexture(shouldRenderToTexture);
        if (renderer->context()->renderContextType() != QSSGRenderContextType::GLES2)
            thePrepResult.flags.setRequiresSsaoPass(SSAOEnabled);

        if (thePrepResult.isLayerVisible()) {
            if (shouldRenderToTexture) {
                renderer->demonContext()->renderList()->addRenderTask(createRenderToTextureRunnable());
            }
            if (layer.lightProbe && checkLightProbeDirty(*layer.lightProbe)) {
                renderer->prepareImageForIbl(*layer.lightProbe);
                wasDataDirty = true;
            }

            bool lightProbeValid = HasValidLightProbe(layer.lightProbe);

            setShaderFeature(QSSGShaderDefines::lightProbe(), lightProbeValid);
            setShaderFeature(QSSGShaderDefines::iblFov(), layer.probeFov < 180.0f);

            if (lightProbeValid && layer.lightProbe2 && checkLightProbeDirty(*layer.lightProbe2)) {
                renderer->prepareImageForIbl(*layer.lightProbe2);
                wasDataDirty = true;
            }

            setShaderFeature(QSSGShaderDefines::lightProbe2(), lightProbeValid && HasValidLightProbe(layer.lightProbe2));

            // Push nodes in reverse depth first order
//            if (renderableNodes.empty()) {
//                camerasAndLights.clear();
//                quint32 dfsIndex = 0;
//                for (QSSGRenderNode *theChild = layer.firstChild; theChild; theChild = theChild->nextSibling)
//                    MaybeQueueNodeForRender(*theChild, renderableNodes, camerasAndLights, dfsIndex);
//                std::reverse(camerasAndLights.begin(), camerasAndLights.end());
//                std::reverse(renderableNodes.begin(), renderableNodes.end());
//                lightToNodeMap.clear();
//            }
            // ### TODO: Really this should only be done if renderableNodes is empty or dirty
            // but we don't have a way to say it's dirty yet (new renderables added to the tree)
            cameras.clear();
            lights.clear();
            renderableNodes.clear();
            quint32 dfsIndex = 0;
            for (QSSGRenderNode *theChild = layer.firstChild; theChild; theChild = theChild->nextSibling)
                MaybeQueueNodeForRender(*theChild, renderableNodes, cameras, lights, dfsIndex);
            std::reverse(cameras.begin(), cameras.end());
            std::reverse(lights.begin(), lights.end());
            std::reverse(renderableNodes.begin(), renderableNodes.end());
            lightToNodeMap.clear();

            camera = nullptr;
            globalLights.clear();
            opaqueObjects.clear();
            qDeleteAll(opaqueObjects);
            transparentObjects.clear();
            qDeleteAll(transparentObjects);
            QVector<QSSGLightNodeMarker> theLightNodeMarkers;
            sourceLightDirections.clear();

            // Cameras
            for (qint32 idx = 0, end = cameras.size(); idx < end; ++idx) {
                QSSGRenderCamera *theCamera = cameras[idx];
                wasDataDirty = wasDataDirty || theCamera->flags.testFlag(QSSGRenderNode::Flag::Dirty);
                QSSGCameraGlobalCalculationResult theResult = thePrepResult.setupCameraForRender(*theCamera);
                wasDataDirty = wasDataDirty || theResult.m_wasDirty;
                if (theCamera->flags.testFlag(QSSGRenderCamera::Flag::GloballyActive))
                    camera = theCamera;
                if (theResult.m_computeFrustumSucceeded == false) {
                    qCCritical(INTERNAL_ERROR, "Failed to calculate camera frustum");
                }
                // If an active camera has been set on the layer, just use that
                if (camera == layer.activeCamera)
                    break;
            }

            // Lights
            for (qint32 idx = 0, end = lights.size(); idx < end; ++idx) {
                QSSGRenderLight *theLight = lights[idx];
                wasDataDirty = wasDataDirty || theLight->flags.testFlag(QSSGRenderNode::Flag::Dirty);
                bool lightResult = theLight->calculateGlobalVariables();
                wasDataDirty = lightResult || wasDataDirty;
                // Note we setup the light index such that it is completely invariant of if
                // the
                // light is active or scoped.
                quint32 lightIndex = (quint32)sourceLightDirections.size();
                sourceLightDirections.push_back(QVector3D(0.0, 0.0, 0.0));
                // Note we still need a light check when building the renderable light list.
                // We also cannot cache shader-light bindings based on layers any more
                // because
                // the number of lights for a given renderable does not depend on the layer
                // as it used to but
                // additional perhaps on the light's scoping rules.
                if (theLight->flags.testFlag(QSSGRenderLight::Flag::GloballyActive)) {
                    if (theLight->m_scope == nullptr) {
                        globalLights.push_back(theLight);
                        if (renderer->context()->renderContextType() != QSSGRenderContextType::GLES2
                                && theLight->m_castShadow) {
                            createShadowMapManager();
                            // PKC -- use of "res" as an exponent of two is an annoying
                            // artifact of the XML interface
                            // I'll change this with an enum interface later on, but that's
                            // less important right now.
                            quint32 mapSize = 1 << theLight->m_shadowMapRes;
                            ShadowMapModes mapMode = (theLight->m_lightType != QSSGRenderLight::Type::Directional)
                                    ? ShadowMapModes::CUBE
                                    : ShadowMapModes::VSM;
                            shadowMapManager->addShadowMapEntry(globalLights.size() - 1,
                                                                mapSize,
                                                                mapSize,
                                                                QSSGRenderTextureFormat::R16F,
                                                                1,
                                                                mapMode,
                                                                ShadowFilterValues::NONE);
                            thePrepResult.flags.setRequiresShadowMapPass(true);
                            setShaderFeature(QSSGShaderDefines::ssm(), true);
                        }
                    }
                    TLightToNodeMap::iterator iter = lightToNodeMap.insert(theLight, (QSSGRenderNode *)nullptr);
                    QSSGRenderNode *oldLightScope = iter.value();
                    QSSGRenderNode *newLightScope = theLight->m_scope;

                    if (oldLightScope != newLightScope) {
                        iter.value() = newLightScope;
                        if (oldLightScope)
                            theLightNodeMarkers.push_back(QSSGLightNodeMarker(*theLight, lightIndex, *oldLightScope, false));
                        if (newLightScope)
                            theLightNodeMarkers.push_back(QSSGLightNodeMarker(*theLight, lightIndex, *newLightScope, true));
                    }
                    if (newLightScope) {
                        sourceLightDirections.back() = theLight->getScalingCorrectDirection();
                    }
                }
            }

            if (theLightNodeMarkers.empty() == false) {
                for (qint32 idx = 0, end = renderableNodes.size(); idx < end; ++idx) {
                    QSSGRenderableNodeEntry &theNodeEntry(renderableNodes[idx]);
                    quint32 nodeDFSIndex = theNodeEntry.node->dfsIndex;
                    for (quint32 markerIdx = 0, markerEnd = theLightNodeMarkers.size(); markerIdx < markerEnd; ++markerIdx) {
                        QSSGLightNodeMarker &theMarker = theLightNodeMarkers[markerIdx];
                        if (nodeDFSIndex >= theMarker.firstValidIndex && nodeDFSIndex < theMarker.justPastLastValidIndex) {
                            if (theMarker.addOrRemove) {
                                QSSGNodeLightEntry *theNewEntry = new QSSGNodeLightEntry(theMarker.light, theMarker.lightIndex);
                                theNodeEntry.lights.push_back(*theNewEntry);
                            } else {
                                for (QSSGNodeLightEntryList::iterator lightIter = theNodeEntry.lights.begin(),
                                                                        lightEnd = theNodeEntry.lights.end();
                                     lightIter != lightEnd;
                                     ++lightIter) {
                                    if (lightIter->light == theMarker.light) {
                                        QSSGNodeLightEntry &theEntry = *lightIter;
                                        theNodeEntry.lights.remove(theEntry);
                                        delete &theEntry;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (camera) {
                camera->calculateViewProjectionMatrix(viewProjection);
                if (camera->enableFrustumClipping) {
                    QSSGClipPlane nearPlane;
                    QMatrix3x3 theUpper33(camera->globalTransform.normalMatrix());

                    QVector3D dir(mat33::transform(theUpper33, QVector3D(0, 0, -1)));
                    dir.normalize();
                    nearPlane.normal = dir;
                    QVector3D theGlobalPos = camera->getGlobalPos() + camera->clipNear * dir;
                    nearPlane.d = -(QVector3D::dotProduct(dir, theGlobalPos));
                    // the near plane's bbox edges are calculated in the clipping frustum's
                    // constructor.
                    clippingFrustum = QSSGClippingFrustum(viewProjection, nearPlane);
                }
            } else
                viewProjection = QMatrix4x4();

            // Setup the light directions here.

            for (qint32 lightIdx = 0, lightEnd = globalLights.size(); lightIdx < lightEnd; ++lightIdx) {
                lightDirections.push_back(globalLights.at(lightIdx)->getScalingCorrectDirection());
            }

            modelContexts.clear();
            if (usesOffscreenRenderer() == false) {
                bool renderablesDirty = prepareRenderablesForRender(viewProjection,
                                                                    clippingFrustum,
                                                                    thePrepResult.flags);
                wasDataDirty = wasDataDirty || renderablesDirty;
                if (thePrepResult.flags.requiresStencilBuffer())
                    thePrepResult.flags.setShouldRenderToTexture(true);
            } else {
                QRect theViewport = thePrepResult.viewport().toRect();
                bool theScissor = true;
                QRect theScissorRect = thePrepResult.scissor().toRect();
                // This happens here because if there are any fancy render steps
                const QSSGRef<QSSGRenderList> &theRenderList(renderer->demonContext()->renderList());
                auto theContext = renderer->context();
                QSSGRenderListScopedProperty<bool> _listScissorEnabled(*theRenderList,
                                                                         &QSSGRenderList::isScissorTestEnabled,
                                                                         &QSSGRenderList::setScissorTestEnabled,
                                                                         theScissor);
                QSSGRenderListScopedProperty<QRect> _listViewport(*theRenderList,
                                                                    &QSSGRenderList::getViewport,
                                                                    &QSSGRenderList::setViewport,
                                                                    theViewport);
                QSSGRenderListScopedProperty<QRect> _listScissor(*theRenderList,
                                                                   &QSSGRenderList::getScissor,
                                                                   &QSSGRenderList::setScissorRect,
                                                                   theScissorRect);
                // Some plugins don't use the render list so they need the actual gl context
                // setup.
                QSSGRenderContextScopedProperty<bool> __scissorEnabled(*theContext,
                                                                         &QSSGRenderContext::isScissorTestEnabled,
                                                                         &QSSGRenderContext::setScissorTestEnabled,
                                                                         true);
                QSSGRenderContextScopedProperty<QRect> __scissorRect(*theContext,
                                                                       &QSSGRenderContext::scissorRect,
                                                                       &QSSGRenderContext::setScissorRect,
                                                                       theScissorRect);
                QSSGRenderContextScopedProperty<QRect> __viewportRect(*theContext,
                                                                        &QSSGRenderContext::viewport,
                                                                        &QSSGRenderContext::setViewport,
                                                                        theViewport);
                QSSGOffscreenRenderFlags theResult = lastFrameOffscreenRenderer
                                                               ->needsRender(createOffscreenRenderEnvironment(),
                                                                             renderer->demonContext()->presentationScaleFactor(),
                                                                             &layer);
                wasDataDirty = wasDataDirty || theResult.hasChangedSinceLastFrame;
            }
        }
    }
    wasDirty = wasDirty || wasDataDirty;
    thePrepResult.flags.setWasDirty(wasDirty);
    thePrepResult.flags.setLayerDataDirty(wasDataDirty);

    layerPrepResult = thePrepResult;

    // Per-frame cache of renderable objects post-sort.
    getOpaqueRenderableObjects();
    // If layer depth test is false, this may also contain opaque objects.
    getTransparentRenderableObjects();

    getCameraDirection();
}

void QSSGLayerRenderPreparationData::resetForFrame()
{
    transparentObjects.clear();
    opaqueObjects.clear();
    layerPrepResult.setEmpty();
    // The check for if the camera is or is not null is used
    // to figure out if this layer was rendered at all.
    camera = nullptr;
    lastFrameOffscreenRenderer = nullptr;
    iRenderWidgets.clear();
    cameraDirection.setEmpty();
    lightDirections.clear();
    renderedOpaqueObjects.clear();
    renderedTransparentObjects.clear();
}

QT_END_NAMESPACE
