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

#ifndef QSSGN_RENDERQSSGDER_TYPES_H
#define QSSGN_RENDERQSSGDER_TYPES_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtQuick3DUtils/private/qssgdataref_p.h>

#include <QtQuick3DRender/private/qtquick3drenderglobal_p.h>
#include <QtQuick3DRender/private/qssgrenderlogging_p.h>

#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>
#include <QFloat16>

#include <cmath>

QT_BEGIN_NAMESPACE

enum class QSSGRenderComponentType
{
    Unknown = 0,
    UnsignedInteger8,
    Integer8,
    UnsignedInteger16,
    Integer16,
    UnsignedInteger32,
    Integer32,
    UnsignedInteger64,
    Integer64,
    Float16,
    Float32,
    Float64
};

inline const char *toString(QSSGRenderComponentType value)
{
    switch (value) {
    case QSSGRenderComponentType::UnsignedInteger8:
        return "UnsignedInteger8";
    case QSSGRenderComponentType::Integer8:
        return "Integer8";
    case QSSGRenderComponentType::UnsignedInteger16:
        return "UnsignedInteger16";
    case QSSGRenderComponentType::Integer16:
        return "Integer16";
    case QSSGRenderComponentType::UnsignedInteger32:
        return "UnsignedInteger32";
    case QSSGRenderComponentType::Integer32:
        return "Integer32";
    case QSSGRenderComponentType::UnsignedInteger64:
        return "UnsignedInteger64";
    case QSSGRenderComponentType::Integer64:
        return "Integer64";
    case QSSGRenderComponentType::Float16:
        return "Float16";
    case QSSGRenderComponentType::Float32:
        return "Float32";
    case QSSGRenderComponentType::Float64:
        return "Float64";
    default:
        break;
    }
    return "Unknown";
}

inline quint32 getSizeOfType(QSSGRenderComponentType value)
{
    switch (value) {
    case QSSGRenderComponentType::UnsignedInteger8:
        return sizeof(quint8);
    case QSSGRenderComponentType::Integer8:
        return sizeof(qint8);
    case QSSGRenderComponentType::UnsignedInteger16:
        return sizeof(quint16);
    case QSSGRenderComponentType::Integer16:
        return sizeof(qint16);
    case QSSGRenderComponentType::UnsignedInteger32:
        return sizeof(quint32);
    case QSSGRenderComponentType::Integer32:
        return sizeof(qint32);
    case QSSGRenderComponentType::UnsignedInteger64:
        return sizeof(quint64);
    case QSSGRenderComponentType::Integer64:
        return sizeof(qint64);
    case QSSGRenderComponentType::Float16:
        return sizeof(qfloat16);
    case QSSGRenderComponentType::Float32:
        return sizeof(float);
    case QSSGRenderComponentType::Float64:
        return sizeof(double);
    default:
        break;
    }
    Q_ASSERT(false);
    return 0;
}

enum class QSSGRenderContextType : quint32
{
    GLES2 = 1 << 0,
    GL2 = 1 << 1,
    GLES3 = 1 << 2,
    GL3 = 1 << 3,
    GLES3PLUS = 1 << 4,
    GL4 = 1 << 5,
    NullContext = 1 << 6,
};

Q_DECLARE_FLAGS(QSSGRenderContextTypes, QSSGRenderContextType)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderContextTypes)

enum class QSSGRenderClearValues : quint32
{
    Color = 1 << 0,
    Depth = 1 << 1,
    Stencil = 1 << 3,
    Coverage = 1 << 4,
};

Q_DECLARE_FLAGS(QSSGRenderClearFlags, QSSGRenderClearValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderClearFlags)

enum class QSSGRenderQueryType
{
    Unknown = 0,
    Samples, ///< samples query object
    Timer, ///< timer query object
};

enum class QSSGRenderQueryResultType
{
    Unknown = 0,
    ResultAvailable, ///< Check if query result is available
    Result, ///< Get actual result
};

enum class QSSGRenderSyncType
{
    Unknown = 0,
    GpuCommandsComplete, ///< sync to Gpu commands finished
};

enum class QSSGRenderSyncValues
{
    Unknown = 0, ///< for future usage
};

Q_DECLARE_FLAGS(QSSGRenderSyncFlags, QSSGRenderSyncValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderSyncFlags)

enum class QSSGRenderCommandFlushValues
{
    SyncFlushCommands = 0, ///< sync for flushing command
};

Q_DECLARE_FLAGS(QSSGRenderCommandFlushFlags, QSSGRenderCommandFlushValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderCommandFlushFlags)

enum class QSSGRenderBufferType : quint32
{
    Vertex, ///< Bind as vertex buffer
    Index, ///< Bind as index buffer
    Constant, ///< Bind as constant buffer
    Storage, ///< Bind as shader storage buffer
    AtomicCounter, ///< Bind as atomic counter buffer
    DrawIndirect, ///< Bind as draw indirect buffer
};

enum class QSSGRenderBufferUsageType
{
    Unknown = 0,
    Static, ///< Rarely updated
    Dynamic, ///< Most likely updated every frame
};

enum class QSSGRenderImageAccessType
{
    Unknown = 0,
    Read, ///< Read only access
    Write, ///< Write only access
    ReadWrite, ///< Read and write access
};

enum class QSSGRenderBufferAccessTypeValues
{
    Unknown = 0,
    Read = 1 << 0, ///< Read access
    Write = 1 << 1, ///< Write access
    Invalid = 1 << 2, ///< No sync
    InvalidRange = 1 << 3, ///< No sync
};

Q_DECLARE_FLAGS(QSSGRenderBufferAccessFlags, QSSGRenderBufferAccessTypeValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderBufferAccessFlags)

///< defines a barrier of ordering the memory transactions to a command relative to those issued
/// before the barrier
enum class QSSGRenderBufferBarrierValues
{
    Unknown = 0,
    VertexAttribArray = 1 << 0, ///< Barrier for vertex attributes sourced from a buffer
    ElementArray = 1 << 1, ///< Barrier for indices sourced from a buffer
    UniformBuffer = 1 << 2, ///< Barrier for shader uniforms sourced from a buffer
    TextureFetch = 1 << 3, ///< Barrier for texture fetches within shaders
    ShaderImageAccess = 1 << 4, ///< Barrier for image access using load / store
    CommandBuffer = 1 << 5, ///< Barrier for indirect drawing
    PixelBuffer = 1 << 6, ///< Barrier for pixel buffer access
    TextureUpdate = 1 << 7, ///< Barrier for texture writes
    BufferUpdate = 1 << 8, ///< Barrier for buffer writes
    Framebuffer = 1 << 9, ///< Barrier for framebuffer writes
    TransformFeedback = 1 << 10, ///< Barrier for transform feedback writes
    AtomicCounter = 1 << 11, ///< Barrier for atomic counter writes
    ShaderStorage = 1 << 12, ///< Barrier for shader storage blocks writes
    All = 0xFFFF, ///< Barrier for all of the above
};

Q_DECLARE_FLAGS(QSSGRenderBufferBarrierFlags, QSSGRenderBufferBarrierValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderBufferBarrierFlags)

enum class QSSGRenderRenderBufferFormat
{
    Unknown = 0,
    RGBA4,
    RGB565,
    RGBA5551,
    Depth16,
    Depth24,
    Depth32,
    StencilIndex8,
    CoverageNV
};

inline const char *toString(QSSGRenderRenderBufferFormat value)
{
    switch (value) {
    case QSSGRenderRenderBufferFormat::RGBA4:
        return "RGBA4";
    case QSSGRenderRenderBufferFormat::RGB565:
        return "RGB565";
    case QSSGRenderRenderBufferFormat::RGBA5551:
        return "RGBA5551";
    case QSSGRenderRenderBufferFormat::Depth16:
        return "Depth16";
    case QSSGRenderRenderBufferFormat::Depth24:
        return "Depth24";
    case QSSGRenderRenderBufferFormat::Depth32:
        return "Depth32";
    case QSSGRenderRenderBufferFormat::StencilIndex8:
        return "StencilIndex8";
    case QSSGRenderRenderBufferFormat::CoverageNV:
        return "CoverageNV";
    default:
        break;
    }
    return "Unknown";
}

struct QSSGRenderTextureFormat
{
    enum Format {
        Unknown = 0,
        R8,
        R16,
        R16F,
        R32I,
        R32UI,
        R32F,
        RG8,
        RGBA8,
        RGB8,
        SRGB8,
        SRGB8A8,
        RGB565,
        RGBA5551,
        Alpha8,
        Luminance8,
        Luminance16,
        LuminanceAlpha8,
        RGBA16F,
        RG16F,
        RG32F,
        RGB32F,
        RGBA32F,
        R11G11B10,
        RGB9E5,
        RGBA_DXT1,
        RGB_DXT1,
        RGBA_DXT3,
        RGBA_DXT5,
        Depth16,
        Depth24,
        Depth32,
        Depth24Stencil8
    };
    Format format;

    constexpr QSSGRenderTextureFormat(Format f) : format(f) {}

    bool isUncompressedTextureFormat() const
    {
        switch (format) {
        case QSSGRenderTextureFormat::R8:
            return true;
        case QSSGRenderTextureFormat::R16:
            return true;
        case QSSGRenderTextureFormat::R16F:
            return true;
        case QSSGRenderTextureFormat::R32I:
            return true;
        case QSSGRenderTextureFormat::R32UI:
            return true;
        case QSSGRenderTextureFormat::R32F:
            return true;
        case QSSGRenderTextureFormat::RG8:
            return true;
        case QSSGRenderTextureFormat::RGBA8:
            return true;
        case QSSGRenderTextureFormat::RGB8:
            return true;
        case QSSGRenderTextureFormat::SRGB8:
            return true;
        case QSSGRenderTextureFormat::SRGB8A8:
            return true;
        case QSSGRenderTextureFormat::RGB565:
            return true;
        case QSSGRenderTextureFormat::RGBA5551:
            return true;
        case QSSGRenderTextureFormat::Alpha8:
            return true;
        case QSSGRenderTextureFormat::Luminance8:
            return true;
        case QSSGRenderTextureFormat::Luminance16:
            return true;
        case QSSGRenderTextureFormat::LuminanceAlpha8:
            return true;
        case QSSGRenderTextureFormat::RGBA16F:
            return true;
        case QSSGRenderTextureFormat::RG16F:
            return true;
        case QSSGRenderTextureFormat::RG32F:
            return true;
        case QSSGRenderTextureFormat::RGB32F:
            return true;
        case QSSGRenderTextureFormat::RGBA32F:
            return true;
        case QSSGRenderTextureFormat::R11G11B10:
            return true;
        case QSSGRenderTextureFormat::RGB9E5:
            return true;
        default:
            break;
        }
        return false;
    }

    bool isCompressedTextureFormat() const
    {
        switch (format) {
        case QSSGRenderTextureFormat::RGBA_DXT1:
            return true;
        case QSSGRenderTextureFormat::RGB_DXT1:
            return true;
        case QSSGRenderTextureFormat::RGBA_DXT3:
            return true;
        case QSSGRenderTextureFormat::RGBA_DXT5:
            return true;
        default:
            break;
        }
        return false;
    }

    bool isDepthTextureFormat() const
    {
        switch (format) {
        case QSSGRenderTextureFormat::Depth16:
            return true;
        case QSSGRenderTextureFormat::Depth24:
            return true;
        case QSSGRenderTextureFormat::Depth32:
            return true;
        case QSSGRenderTextureFormat::Depth24Stencil8:
            return true;
        default:
            break;
        }
        return false;
    }

    const char *toString() const
    {
        switch (format) {
        case R8:
            return "R8";
        case R16:
            return "R16";
        case R16F:
            return "R16F";
        case R32I:
            return "R32I";
        case R32UI:
            return "R32UI";
        case R32F:
            return "R32F";
        case RG8:
            return "RG8";
        case RGBA8:
            return "RGBA8";
        case RGB8:
            return "RGB8";
        case SRGB8:
            return "SRGB8";
        case SRGB8A8:
            return "SRGB8A8";
        case RGB565:
            return "RGB565";
        case RGBA5551:
            return "RGBA5551";
        case Alpha8:
            return "Alpha8";
        case Luminance8:
            return "Luminance8";
        case Luminance16:
            return "Luminance16";
        case LuminanceAlpha8:
            return "LuminanceAlpha8";
        case RGBA16F:
            return "RGBA16F";
        case RG16F:
            return "RG16F";
        case RG32F:
            return "RG32F";
        case RGB32F:
            return "RGB32F";
        case RGBA32F:
            return "RGBA32F";
        case R11G11B10:
            return "R11G11B10";
        case RGB9E5:
            return "RGB9E5";
        case RGBA_DXT1:
            return "RGBA_DXT1";
        case RGB_DXT1:
            return "RGB_DXT1";
        case RGBA_DXT3:
            return "RGBA_DXT3";
        case RGBA_DXT5:
            return "RGBA_DXT5";
        case Depth16:
            return "Depth16";
        case Depth24:
            return "Depth24";
        case Depth32:
            return "Depth32";
        case Depth24Stencil8:
            return "Depth24Stencil8";
        default:
            break;
        }
        return "Unknown";
    }

    qint32 getSizeofFormat() const
    {
        switch (format) {
        case R8:
            return 1;
        case R16F:
            return 2;
        case R16:
            return 2;
        case R32I:
            return 4;
        case R32F:
            return 4;
        case RGBA8:
            return 4;
        case RGB8:
            return 3;
        case RGB565:
            return 2;
        case RGBA5551:
            return 2;
        case Alpha8:
            return 1;
        case Luminance8:
            return 1;
        case LuminanceAlpha8:
            return 1;
        case Depth16:
            return 2;
        case Depth24:
            return 3;
        case Depth32:
            return 4;
        case Depth24Stencil8:
            return 4;
        case RGB9E5:
            return 4;
        case SRGB8:
            return 3;
        case SRGB8A8:
            return 4;
        case RGBA16F:
            return 8;
        case RG16F:
            return 4;
        case RG32F:
            return 8;
        case RGBA32F:
            return 16;
        case RGB32F:
            return 12;
        case R11G11B10:
            return 4;
        default:
            break;
        }
        Q_ASSERT(false);
        return 0;
    }

    qint32 getNumberOfComponent() const
    {
        switch (format) {
        case R8:
            return 1;
        case R16F:
            return 1;
        case R16:
            return 1;
        case R32I:
            return 1;
        case R32F:
            return 1;
        case RGBA8:
            return 4;
        case RGB8:
            return 3;
        case RGB565:
            return 3;
        case RGBA5551:
            return 4;
        case Alpha8:
            return 1;
        case Luminance8:
            return 1;
        case LuminanceAlpha8:
            return 2;
        case Depth16:
            return 1;
        case Depth24:
            return 1;
        case Depth32:
            return 1;
        case Depth24Stencil8:
            return 2;
        case RGB9E5:
            return 3;
        case SRGB8:
            return 3;
        case SRGB8A8:
            return 4;
        case RGBA16F:
            return 4;
        case RG16F:
            return 2;
        case RG32F:
            return 2;
        case RGBA32F:
            return 4;
        case RGB32F:
            return 3;
        case R11G11B10:
            return 3;
        default:
            break;
        }
        Q_ASSERT(false);
        return 0;
    }

    void decodeToFloat(void *inPtr, qint32 byteOfs, float *outPtr) const
    {
        Q_ASSERT(byteOfs >= 0);
        outPtr[0] = 0.0f;
        outPtr[1] = 0.0f;
        outPtr[2] = 0.0f;
        outPtr[3] = 0.0f;
        quint8 *src = reinterpret_cast<quint8 *>(inPtr);
        // float divisor;		// If we want to support RGBD?
        switch (format) {
        case Alpha8:
            outPtr[0] = (float(src[byteOfs])) / 255.0f;
            break;

        case Luminance8:
        case LuminanceAlpha8:
        case R8:
        case RG8:
        case RGB8:
        case RGBA8:
        case SRGB8:
        case SRGB8A8:
            // NOTE : RGBD Hack here for reference.  Not meant for installation.
            // divisor = (QSSGRenderTextureFormat::getSizeofFormat(inFmt) == 4) ?
            // ((float)src[byteOfs+3]) / 255.0f : 1.0f;
            for (qint32 i = 0; i < getSizeofFormat(); ++i) {
                float val = (float(src[byteOfs + i])) / 255.0f;
                outPtr[i] = (i < 3) ? std::pow(val, 0.4545454545f) : val;
                // Assuming RGBA8 actually means RGBD (which is stupid, I know)
                // if ( QSSGRenderTextureFormat::getSizeofFormat(inFmt) == 4 ) { outPtr[i] /=
                // divisor; }
            }
            // outPtr[3] = divisor;
            break;

        case R32F:
            outPtr[0] = reinterpret_cast<float *>(src + byteOfs)[0];
            break;
        case RG32F:
            outPtr[0] = reinterpret_cast<float *>(src + byteOfs)[0];
            outPtr[1] = reinterpret_cast<float *>(src + byteOfs)[1];
            break;
        case RGBA32F:
            outPtr[0] = reinterpret_cast<float *>(src + byteOfs)[0];
            outPtr[1] = reinterpret_cast<float *>(src + byteOfs)[1];
            outPtr[2] = reinterpret_cast<float *>(src + byteOfs)[2];
            outPtr[3] = reinterpret_cast<float *>(src + byteOfs)[3];
            break;
        case RGB32F:
            outPtr[0] = reinterpret_cast<float *>(src + byteOfs)[0];
            outPtr[1] = reinterpret_cast<float *>(src + byteOfs)[1];
            outPtr[2] = reinterpret_cast<float *>(src + byteOfs)[2];
            break;

        case R16F:
        case RG16F:
        case RGBA16F:
            for (qint32 i = 0; i < (getSizeofFormat() >> 1); ++i) {
                // NOTE : This only works on the assumption that we don't have any denormals,
                // Infs or NaNs.
                // Every pixel in our source image should be "regular"
                quint16 h = reinterpret_cast<quint16 *>(src + byteOfs)[i];
                quint32 sign = (h & 0x8000u) << 16u;
                quint32 exponent = (((((h & 0x7c00u) >> 10) - 15) + 127) << 23);
                quint32 mantissa = ((h & 0x3ffu) << 13);
                quint32 result = sign | exponent | mantissa;

                if (h == 0 || h == 0x8000) {
                    result = 0;
                } // Special case for zero and negative zero
                memcpy(reinterpret_cast<quint32 *>(outPtr) + i, &result, 4);
            }
            break;

        case R11G11B10:
            // place holder
            Q_ASSERT(false);
            break;

        default:
            outPtr[0] = 0.0f;
            outPtr[1] = 0.0f;
            outPtr[2] = 0.0f;
            outPtr[3] = 0.0f;
            break;
        }
    }

    void encodeToPixel(float *inPtr, void *outPtr, qint32 byteOfs) const
    {
        Q_ASSERT(byteOfs >= 0);
        quint8 *dest = reinterpret_cast<quint8 *>(outPtr);
        switch (format) {
        case QSSGRenderTextureFormat::Alpha8:
            dest[byteOfs] = quint8(inPtr[0] * 255.0f);
            break;

        case Luminance8:
        case LuminanceAlpha8:
        case R8:
        case RG8:
        case RGB8:
        case RGBA8:
        case SRGB8:
        case SRGB8A8:
            for (qint32 i = 0; i < getSizeofFormat(); ++i) {
                inPtr[i] = (inPtr[i] > 1.0f) ? 1.0f : inPtr[i];
                if (i < 3)
                    dest[byteOfs + i] = quint8(powf(inPtr[i], 2.2f) * 255.0f);
                else
                    dest[byteOfs + i] = quint8(inPtr[i] * 255.0f);
            }
            break;

        case R32F:
            reinterpret_cast<float *>(dest + byteOfs)[0] = inPtr[0];
            break;
        case RG32F:
            reinterpret_cast<float *>(dest + byteOfs)[0] = inPtr[0];
            reinterpret_cast<float *>(dest + byteOfs)[1] = inPtr[1];
            break;
        case RGBA32F:
            reinterpret_cast<float *>(dest + byteOfs)[0] = inPtr[0];
            reinterpret_cast<float *>(dest + byteOfs)[1] = inPtr[1];
            reinterpret_cast<float *>(dest + byteOfs)[2] = inPtr[2];
            reinterpret_cast<float *>(dest + byteOfs)[3] = inPtr[3];
            break;
        case RGB32F:
            reinterpret_cast<float *>(dest + byteOfs)[0] = inPtr[0];
            reinterpret_cast<float *>(dest + byteOfs)[1] = inPtr[1];
            reinterpret_cast<float *>(dest + byteOfs)[2] = inPtr[2];
            break;

        case R16F:
        case RG16F:
        case RGBA16F:
            for (qint32 i = 0; i < (getSizeofFormat() >> 1); ++i) {
                // NOTE : This also has the limitation of not handling  infs, NaNs and
                // denormals, but it should be
                // sufficient for our purposes.
                if (inPtr[i] > 65519.0f) {
                    inPtr[i] = 65519.0f;
                }
                if (std::fabs(inPtr[i]) < 6.10352E-5f) {
                    inPtr[i] = 0.0f;
                }
                quint32 f = reinterpret_cast<quint32 *>(inPtr)[i];
                quint32 sign = (f & 0x80000000) >> 16;
                qint32 exponent = (f & 0x7f800000) >> 23;
                quint32 mantissa = (f >> 13) & 0x3ff;
                exponent = exponent - 112;
                if (exponent > 31) {
                    exponent = 31;
                }
                if (exponent < 0) {
                    exponent = 0;
                }
                exponent = exponent << 10;
                reinterpret_cast<quint16 *>(dest + byteOfs)[i] = quint16(sign | quint32(exponent) | mantissa);
            }
            break;

        case R11G11B10:
            // place holder
            Q_ASSERT(false);
            break;

        default:
            dest[byteOfs] = 0;
            dest[byteOfs + 1] = 0;
            dest[byteOfs + 2] = 0;
            dest[byteOfs + 3] = 0;
            break;
        }
    }

    bool operator==(const QSSGRenderTextureFormat &other) const { return format == other.format; }
    bool operator!=(const QSSGRenderTextureFormat &other) const { return format != other.format; }
};

enum class QSSGRenderTextureTargetType
{
    Unknown = 0,
    Texture2D,
    Texture2D_MS,
    Texture2D_Array,
    TextureCube,
    TextureCubePosX,
    TextureCubeNegX,
    TextureCubePosY,
    TextureCubeNegY,
    TextureCubePosZ,
    TextureCubeNegZ,
};

enum class QSSGRenderTextureUnit
{
    TextureUnit_0 = 0,
    TextureUnit_1,
    TextureUnit_2,
    TextureUnit_3,
    TextureUnit_4,
    TextureUnit_5,
    TextureUnit_6,
    TextureUnit_7,
    TextureUnit_8,
    TextureUnit_9,
    TextureUnit_10,
    TextureUnit_11,
    TextureUnit_12,
    TextureUnit_13,
    TextureUnit_14,
    TextureUnit_15,
    TextureUnit_16,
    TextureUnit_17,
    TextureUnit_18,
    TextureUnit_19,
    TextureUnit_20,
    TextureUnit_21,
    TextureUnit_22,
    TextureUnit_23,
    TextureUnit_24,
    TextureUnit_25,
    TextureUnit_26,
    TextureUnit_27,
    TextureUnit_28,
    TextureUnit_29,
    TextureUnit_30,
    TextureUnit_31
};

enum class QSSGRenderTextureCompareMode
{
    Unknown = 0,
    NoCompare,
    CompareToRef
};

enum class QSSGRenderTextureSwizzleMode
{
    NoSwizzle = 0,
    L8toR8,
    A8toR8,
    L8A8toRG8,
    L16toR16
};

enum class QSSGRenderTextureCompareOp
{
    Never,
    Less,
    LessThanOrEqual,
    Equal,
    NotEqual,
    Greater,
    GreaterThanOrEqual,
    AlwaysTrue,
};

enum class QSSGRenderTextureMinifyingOp
{
    Unknown = 0,
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};
inline const char *toString(QSSGRenderTextureMinifyingOp value)
{
    switch (value) {
    case QSSGRenderTextureMinifyingOp::Nearest:
        return "Nearest";
    case QSSGRenderTextureMinifyingOp::Linear:
        return "Linear";
    case QSSGRenderTextureMinifyingOp::NearestMipmapNearest:
        return "NearestMipmapNearest";
    case QSSGRenderTextureMinifyingOp::LinearMipmapNearest:
        return "LinearMipmapNearest";
    case QSSGRenderTextureMinifyingOp::NearestMipmapLinear:
        return "NearestMipmapLinear";
    case QSSGRenderTextureMinifyingOp::LinearMipmapLinear:
        return "LinearMipmapLinear";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderTextureMagnifyingOp
{
    Unknown = 0,
    Nearest,
    Linear
};
inline const char *toString(QSSGRenderTextureMagnifyingOp value)
{
    switch (value) {
    case QSSGRenderTextureMagnifyingOp::Nearest:
        return "Nearest";
    case QSSGRenderTextureMagnifyingOp::Linear:
        return "Linear";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderTextureCoordOp
{

    Unknown = 0,
    ClampToEdge,
    MirroredRepeat,
    Repeat
};
inline const char *toString(QSSGRenderTextureCoordOp value)
{
    switch (value) {
    case QSSGRenderTextureCoordOp::ClampToEdge:
        return "ClampToEdge";
    case QSSGRenderTextureCoordOp::MirroredRepeat:
        return "MirroredRepeat";
    case QSSGRenderTextureCoordOp::Repeat:
        return "Repeat";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderHint
{
    Unknown = 0,
    Fastest,
    Nicest,
    Unspecified
};
inline const char *toString(QSSGRenderHint value)
{
    switch (value) {
    case QSSGRenderHint::Fastest:
        return "Fastest";
    case QSSGRenderHint::Nicest:
        return "Nicest";
    case QSSGRenderHint::Unspecified:
        return "Unspecified";
    default:
        break;
    }
    return "Unknown";
}

struct QSSGRenderVertexBufferEntry
{
    const char *m_name;
    /** Datatype of the this entry points to in the buffer */
    QSSGRenderComponentType m_componentType;
    /** Number of components of each data member. 1,2,3, or 4.  Don't be stupid.*/
    quint32 m_numComponents;
    /** Offset from the beginning of the buffer of the first item */
    quint32 m_firstItemOffset;
    /** Attribute input slot used for this entry*/
    quint32 m_inputSlot;

    QSSGRenderVertexBufferEntry(const char *nm,
                                  QSSGRenderComponentType type,
                                  quint32 numComponents,
                                  quint32 firstItemOffset = 0,
                                  quint32 inputSlot = 0)
        : m_name(nm), m_componentType(type), m_numComponents(numComponents), m_firstItemOffset(firstItemOffset), m_inputSlot(inputSlot)
    {
    }

    QSSGRenderVertexBufferEntry()
        : m_name(nullptr), m_componentType(QSSGRenderComponentType::Unknown), m_numComponents(0), m_firstItemOffset(0), m_inputSlot(0)
    {
    }

    QSSGRenderVertexBufferEntry(const QSSGRenderVertexBufferEntry &inOther)
        : m_name(inOther.m_name)
        , m_componentType(inOther.m_componentType)
        , m_numComponents(inOther.m_numComponents)
        , m_firstItemOffset(inOther.m_firstItemOffset)
        , m_inputSlot(inOther.m_inputSlot)
    {
    }

    QSSGRenderVertexBufferEntry &operator=(const QSSGRenderVertexBufferEntry &inOther)
    {
        if (this != &inOther) {
            m_name = inOther.m_name;
            m_componentType = inOther.m_componentType;
            m_numComponents = inOther.m_numComponents;
            m_firstItemOffset = inOther.m_firstItemOffset;
            m_inputSlot = inOther.m_inputSlot;
        }
        return *this;
    }
};

class QSSGRenderShaderProgram;

// typedef QSSGConstDataRef<qint8> TConstI8Ref;

struct Q_QUICK3DRENDER_EXPORT QSSGRenderVertFragCompilationResult
{
    const char *m_shaderName = nullptr;
    QSSGRef<QSSGRenderShaderProgram> m_shader; ///< contains the program

    QSSGRenderVertFragCompilationResult();
    ~QSSGRenderVertFragCompilationResult();
    QSSGRenderVertFragCompilationResult(const QSSGRenderVertFragCompilationResult &);
    QSSGRenderVertFragCompilationResult & operator=(const QSSGRenderVertFragCompilationResult &other);
};

enum class QSSGRenderFrameBufferAttachment
{
    Unknown = 0,
    Color0,
    Color1,
    Color2,
    Color3,
    Color4,
    Color5,
    Color6,
    Color7,
    Depth,
    Stencil,
    DepthStencil,
    CoverageNV,
    LastAttachment,
};

enum class QSSGRenderDrawMode
{
    Unknown = 0,
    Points,
    LineStrip,
    LineLoop,
    Lines,
    TriangleStrip,
    TriangleFan,
    Triangles,
    Patches,
};

enum class QSSGRenderTextureCubeFace
{
    InvalidFace = 0,
    CubePosX = 1,
    CubeNegX,
    CubePosY,
    CubeNegY,
    CubePosZ,
    CubeNegZ
};

// enums match the NV path extensions
enum class QSSGRenderPathCommands : quint8
{
    Close = 0,
    MoveTo = 2,
    CubicCurveTo = 12,
};

enum class QSSGRenderPathFontTarget
{
    StandardFont = 0,
    SystemFont = 1,
    FileFont = 2,
};

enum class QSSGRenderPathMissingGlyphs
{
    SkipMissing = 0,
    UseMissing = 1,
};

enum class QSSGRenderPathFontStyleValue
{
    Bold = 1 << 0,
    Italic = 1 << 1,
};

Q_DECLARE_FLAGS(QSSGRenderPathFontStyleFlags, QSSGRenderPathFontStyleValue)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderPathFontStyleFlags)

enum class QSSGRenderPathReturnValues
{
    FontGlypsAvailable = 0,
    FontTargetUnavailable = 1,
    FontUnavailable = 2,
    FontUnintelligible = 3,
    InvalidEnum = 4,
    OutOfMemory = 5,
};

enum class QSSGRenderPathFormatType
{
    Byte = 1,
    UByte,
    Short,
    UShort,
    Int,
    Uint,
    Float,
    Utf8,
    Utf16,
    Bytes2,
    Bytes3,
    Bytes4,
};

enum class QSSGRenderPathGlyphFontMetricValues
{
    GlyphWidth = 1 << 0,
    GlyphHeight = 1 << 1,
    GlyphHorizontalBearingX = 1 << 2,
    GlyphHorizontalBearingY = 1 << 3,
    GlyphHorizontalBearingAdvance = 1 << 4,
    GlyphVerticalBearingX = 1 << 5,
    GlyphVerticalBearingY = 1 << 6,
    GlyphVerticalBearingAdvance = 1 << 7,
    GlyphHasKerning = 1 << 8,

    FontXMinBounds = 1 << 9,
    FontYMinBounds = 1 << 10,
    FontXMaxBounds = 1 << 11,
    FontYMaxBounds = 1 << 12,
    FontUnitsPerEm = 1 << 13,
    FontAscender = 1 << 14,
    FontDescender = 1 << 15,
    FontHeight = 1 << 16,
    FontMaxAdvanceWidth = 1 << 17,
    FontMaxAdvanceHeight = 1 << 18,
    FontUnderlinePosition = 1 << 19,
    FontUnderlineThickness = 1 << 20,
    FontHasKerning = 1 << 21,
    FontNumGlyphIndices = 1 << 22,
};

Q_DECLARE_FLAGS(QSSGRenderPathGlyphFontMetricFlags, QSSGRenderPathGlyphFontMetricValues)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderPathGlyphFontMetricFlags)

enum class QSSGRenderPathListMode
{
    AccumAdjacentPairs = 1,
    AdjacentPairs,
    FirstToRest,
};

enum class QSSGRenderPathFillMode
{
    Fill = 1,
    CountUp,
    CountDown,
    Invert,
};

enum class QSSGRenderPathCoverMode
{
    ConvexHull = 1,
    BoundingBox,
    BoundingBoxOfBoundingBox,
    PathFillCover,
    PathStrokeCover,
};

enum class QSSGRenderPathTransformType
{
    NoTransform = 0,
    TranslateX,
    TranslateY,
    Translate2D,
    Translate3D,
    Affine2D,
    Affine3D,
    TransposeAffine2D,
    TransposeAffine3D,
};

enum class QSSGRenderWinding
{
    Unknown = 0,
    Clockwise,
    CounterClockwise
};

inline const char *toString(QSSGRenderWinding value)
{
    switch (value) {
    case QSSGRenderWinding::Clockwise:
        return "Clockwise";
    case QSSGRenderWinding::CounterClockwise:
        return "CounterClockwise";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderState
{
    Unknown = 0,
    Blend,
    CullFace,
    DepthTest,
    StencilTest,
    ScissorTest,
    DepthWrite,
    Multisample
};

inline const char *toString(QSSGRenderState value)
{
    switch (value) {
    case QSSGRenderState::Blend:
        return "Blend";
    case QSSGRenderState::CullFace:
        return "CullFace";
    case QSSGRenderState::DepthTest:
        return "DepthTest";
    case QSSGRenderState::StencilTest:
        return "StencilTest";
    case QSSGRenderState::ScissorTest:
        return "ScissorTest";
    case QSSGRenderState::DepthWrite:
        return "DepthWrite";
    case QSSGRenderState::Multisample:
        return "Multisample";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderSrcBlendFunc
{
    Unknown = 0,
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SrcAlphaSaturate
};

inline const char *toString(QSSGRenderSrcBlendFunc value)
{
    switch (value) {
    case QSSGRenderSrcBlendFunc::Zero:
        return "Zero";
    case QSSGRenderSrcBlendFunc::One:
        return "One";
    case QSSGRenderSrcBlendFunc::SrcColor:
        return "SrcColor";
    case QSSGRenderSrcBlendFunc::OneMinusSrcColor:
        return "OneMinusSrcColor";
    case QSSGRenderSrcBlendFunc::DstColor:
        return "DstColor";
    case QSSGRenderSrcBlendFunc::OneMinusDstColor:
        return "OneMinusDstColor";
    case QSSGRenderSrcBlendFunc::SrcAlpha:
        return "SrcAlpha";
    case QSSGRenderSrcBlendFunc::OneMinusSrcAlpha:
        return "OneMinusSrcAlpha";
    case QSSGRenderSrcBlendFunc::DstAlpha:
        return "DstAlpha";
    case QSSGRenderSrcBlendFunc::OneMinusDstAlpha:
        return "OneMinusDstAlpha";
    case QSSGRenderSrcBlendFunc::ConstantColor:
        return "ConstantColor";
    case QSSGRenderSrcBlendFunc::OneMinusConstantColor:
        return "OneMinusConstantColor";
    case QSSGRenderSrcBlendFunc::ConstantAlpha:
        return "ConstantAlpha";
    case QSSGRenderSrcBlendFunc::OneMinusConstantAlpha:
        return "OneMinusConstantAlpha";
    case QSSGRenderSrcBlendFunc::SrcAlphaSaturate:
        return "SrcAlphaSaturate";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderDstBlendFunc
{
    Unknown = 0,
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha
};

inline const char *toString(QSSGRenderDstBlendFunc value)
{
    return toString(static_cast<QSSGRenderSrcBlendFunc>(value));
}

enum class QSSGRenderBlendEquation
{
    Unknown = 0,
    Add,
    Subtract,
    ReverseSubtract,
    Overlay,
    ColorBurn,
    ColorDodge
};

inline const char *toString(QSSGRenderBlendEquation value)
{
    switch (value) {
    case QSSGRenderBlendEquation::Add:
        return "Add";
    case QSSGRenderBlendEquation::Subtract:
        return "Subtract";
    case QSSGRenderBlendEquation::ReverseSubtract:
        return "ReverseSubtract";
    case QSSGRenderBlendEquation::Overlay:
        return "Overlay";
    case QSSGRenderBlendEquation::ColorBurn:
        return "ColorBurn";
    case QSSGRenderBlendEquation::ColorDodge:
        return "ColorDodge";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderFace
{
    Unknown = 0,
    Front,
    Back,
    FrontAndBack
};

inline const char *toString(QSSGRenderFace value)
{
    switch (value) {
    case QSSGRenderFace::Front:
        return "Front";
    case QSSGRenderFace::Back:
        return "Back";
    case QSSGRenderFace::FrontAndBack:
        return "FrontAndBack";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGReadFace
{
    Unknown = 0,
    Front,
    Back,
    Color0,
    Color1,
    Color2,
    Color3,
    Color4,
    Color5,
    Color6,
    Color7
};

inline const char *toString(QSSGReadFace value)
{
    switch (value) {
    case QSSGReadFace::Front:
        return "Front";
    case QSSGReadFace::Back:
        return "Back";
    case QSSGReadFace::Color0:
        return "Color0";
    case QSSGReadFace::Color1:
        return "Color1";
    case QSSGReadFace::Color2:
        return "Color2";
    case QSSGReadFace::Color3:
        return "Color3";
    case QSSGReadFace::Color4:
        return "Color4";
    case QSSGReadFace::Color5:
        return "Color5";
    case QSSGReadFace::Color6:
        return "Color6";
    case QSSGReadFace::Color7:
        return "Color7";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderBoolOp
{

    Unknown = 0,
    Never,
    Less,
    LessThanOrEqual,
    Equal,
    NotEqual,
    Greater,
    GreaterThanOrEqual,
    AlwaysTrue
};

inline const char *toString(QSSGRenderBoolOp value)
{
    switch (value) {
    case QSSGRenderBoolOp::Never:
        return "Never";
    case QSSGRenderBoolOp::Less:
        return "Less";
    case QSSGRenderBoolOp::LessThanOrEqual:
        return "LessThanOrEqual";
    case QSSGRenderBoolOp::Equal:
        return "Equal";
    case QSSGRenderBoolOp::NotEqual:
        return "NotEqual";
    case QSSGRenderBoolOp::Greater:
        return "Greater";
    case QSSGRenderBoolOp::GreaterThanOrEqual:
        return "GreaterThanOrEqual";
    case QSSGRenderBoolOp::AlwaysTrue:
        return "AlwaysTrue";
    default:
        break;
    }
    return "Unknown";
}

enum class QSSGRenderStencilOp
{
    Unknown = 0,
    Keep,
    Zero,
    Replace,
    Increment,
    IncrementWrap,
    Decrement,
    DecrementWrap,
    Invert
};

inline const char *toString(QSSGRenderStencilOp value)
{
    switch (value) {
    case QSSGRenderStencilOp::Keep:
        return "Keep";
    case QSSGRenderStencilOp::Zero:
        return "Zero";
    case QSSGRenderStencilOp::Replace:
        return "Replace";
    case QSSGRenderStencilOp::Increment:
        return "Increment";
    case QSSGRenderStencilOp::IncrementWrap:
        return "IncrementWrap";
    case QSSGRenderStencilOp::Decrement:
        return "Decrement";
    case QSSGRenderStencilOp::DecrementWrap:
        return "DecrementWrap";
    case QSSGRenderStencilOp::Invert:
        return "Invert";
    default:
        break;
    }
    return "Unknown";
}

struct QSSGRenderBlendFunctionArgument
{
    QSSGRenderSrcBlendFunc m_srcRgb;
    QSSGRenderDstBlendFunc m_dstRgb;
    QSSGRenderSrcBlendFunc m_srcAlpha;
    QSSGRenderDstBlendFunc m_dstAlpha;

    QSSGRenderBlendFunctionArgument(QSSGRenderSrcBlendFunc srcRGB,
                                      QSSGRenderDstBlendFunc dstRGB,
                                      QSSGRenderSrcBlendFunc srcAlpha,
                                      QSSGRenderDstBlendFunc dstAlpha)
        : m_srcRgb(srcRGB), m_dstRgb(dstRGB), m_srcAlpha(srcAlpha), m_dstAlpha(dstAlpha)
    {
    }

    // Default blend system premultiplies values.
    QSSGRenderBlendFunctionArgument()
        : m_srcRgb(QSSGRenderSrcBlendFunc::SrcAlpha)
        , m_dstRgb(QSSGRenderDstBlendFunc::OneMinusSrcAlpha)
        , m_srcAlpha(QSSGRenderSrcBlendFunc::One)
        , m_dstAlpha(QSSGRenderDstBlendFunc::OneMinusSrcAlpha)
    {
    }
};

struct QSSGRenderBlendEquationArgument
{
    QSSGRenderBlendEquation m_rgbEquation;
    QSSGRenderBlendEquation m_alphaEquation;

    QSSGRenderBlendEquationArgument(QSSGRenderBlendEquation rgb, QSSGRenderBlendEquation alpha)
        : m_rgbEquation(rgb), m_alphaEquation(alpha)
    {
    }
    QSSGRenderBlendEquationArgument()
        : m_rgbEquation(QSSGRenderBlendEquation::Add), m_alphaEquation(QSSGRenderBlendEquation::Add)
    {
    }
};

struct QSSGRenderStencilOperation
{
    QSSGRenderStencilOp m_stencilFail = QSSGRenderStencilOp::Keep; // What happens when stencil test fails.
    // These values assume the stencil passed

    // What happens when the stencil passes but depth test fail.
    QSSGRenderStencilOp m_depthFail = QSSGRenderStencilOp::Keep;
     // What happens when the stencil and depth tests pass.
    QSSGRenderStencilOp m_depthPass = QSSGRenderStencilOp::Keep;

    QSSGRenderStencilOperation(QSSGRenderStencilOp fail,
                                         QSSGRenderStencilOp depthFail,
                                         QSSGRenderStencilOp depthPass)
        : m_stencilFail(fail), m_depthFail(depthFail), m_depthPass(depthPass)
    {
    }
    QSSGRenderStencilOperation() = default;

    bool operator==(const QSSGRenderStencilOperation &other) const
    {
        return (m_stencilFail == other.m_stencilFail && m_depthFail == other.m_depthFail && m_depthPass == other.m_depthPass);
    }
};

// see glStencilFuncSeparate
struct QSSGRenderStencilFunction
{
    QSSGRenderBoolOp m_function = QSSGRenderBoolOp::AlwaysTrue;
    quint32 m_referenceValue = 0;
    quint32 m_mask = std::numeric_limits<quint32>::max();

    QSSGRenderStencilFunction(QSSGRenderBoolOp function, quint32 referenceValue, quint32 mask)
        : m_function(function), m_referenceValue(referenceValue), m_mask(mask)
    {
    }
    QSSGRenderStencilFunction() = default;

    bool operator==(const QSSGRenderStencilFunction &other) const
    {
        return (m_function == other.m_function && m_referenceValue == other.m_referenceValue && m_mask == other.m_mask);
    }
};

class QSSGRenderFrameBuffer;
class QSSGRenderVertexBuffer;
class QSSGRenderIndexBuffer;
class QSSGRenderProgramPipeline;
class QSSGRenderTextureBase;
class QSSGRenderTexture2D;
class QSSGRenderTexture2DArray;
class QSSGRenderTextureCube;
class QSSGRenderImage2D;
class QSSGRenderDataBuffer;
class QSSGRenderAttribLayout;
class QSSGRenderInputAssembler;

// Return coordinates in pixels but relative to this rect.
static inline constexpr QVector2D toRectRelative(const QRectF &r, const QVector2D &absoluteCoordinates)
{
    return QVector2D(absoluteCoordinates.x() - float(r.x()), absoluteCoordinates.y() - float(r.y()));
}

static inline constexpr QVector2D halfDims(const QRectF &r)
{
    return QVector2D(float(r.width() / 2.0), float(r.height() / 2.0));
}

// Take coordinates in global space and move local space where 0,0 is the center
// of the rect but return value in pixels, not in normalized -1,1 range
inline QVector2D toNormalizedRectRelative(const QRectF &r, QVector2D absoluteCoordinates)
{
    // normalize them
    const QVector2D relativeCoords(toRectRelative(r, absoluteCoordinates));
    const QVector2D halfD(halfDims(r));
    const QVector2D normalized((relativeCoords.x() / halfD.x()) - 1.0f, (relativeCoords.y() / halfD.y()) - 1.0f);
    return QVector2D(normalized.x() * halfD.x(), normalized.y() * halfD.y());
}

static inline constexpr QVector2D relativeToNormalizedCoordinates(const QRectF &r, QVector2D rectRelativeCoords)
{
    return { (rectRelativeCoords.x() / halfDims(r).x()) - 1.0f, (rectRelativeCoords.y() / halfDims(r).y()) - 1.0f };
}

// Normalized coordinates are in the range of -1,1 where -1 is the left, bottom edges
// and 1 is the top,right edges.
inline constexpr QVector2D absoluteToNormalizedCoordinates(const QRectF &r, const QVector2D &absoluteCoordinates)
{
    return relativeToNormalizedCoordinates(r, toRectRelative(r, absoluteCoordinates));
}

inline constexpr QVector2D toAbsoluteCoords(const QRectF &r, const QVector2D &inRelativeCoords)
{
    return QVector2D(inRelativeCoords.x() + float(r.x()), inRelativeCoords.y() + float(r.y()));
}

template<typename TDataType>
struct QSSGRenderGenericVec2
{
    TDataType x;
    TDataType y;
    QSSGRenderGenericVec2(TDataType _x, TDataType _y) : x(_x), y(_y) {}
    QSSGRenderGenericVec2() {}
    bool operator==(const QSSGRenderGenericVec2 &inOther) const { return x == inOther.x && y == inOther.y; }
};

template<typename TDataType>
struct QSSGRenderGenericVec3
{
    TDataType x;
    TDataType y;
    TDataType z;
    QSSGRenderGenericVec3(TDataType _x, TDataType _y, TDataType _z) : x(_x), y(_y), z(_z) {}
    QSSGRenderGenericVec3() {}
    bool operator==(const QSSGRenderGenericVec3 &inOther) const
    {
        return x == inOther.x && y == inOther.y && z == inOther.z;
    }
};

template<typename TDataType>
struct QSSGRenderGenericVec4
{
    TDataType x;
    TDataType y;
    TDataType z;
    TDataType w;
    QSSGRenderGenericVec4(TDataType _x, TDataType _y, TDataType _z, TDataType _w) : x(_x), y(_y), z(_z), w(_w) {}
    QSSGRenderGenericVec4() {}
    bool operator==(const QSSGRenderGenericVec4 &inOther) const
    {
        return x == inOther.x && y == inOther.y && z == inOther.z && w == inOther.w;
    }
};

typedef QSSGRenderGenericVec2<bool> bool_2;
typedef QSSGRenderGenericVec3<bool> bool_3;
typedef QSSGRenderGenericVec4<bool> bool_4;
typedef QSSGRenderGenericVec2<quint32> quint32_2;
typedef QSSGRenderGenericVec3<quint32> quint32_3;
typedef QSSGRenderGenericVec4<quint32> quint32_4;
typedef QSSGRenderGenericVec2<qint32> qint32_2;
typedef QSSGRenderGenericVec3<qint32> qint32_3;
typedef QSSGRenderGenericVec4<qint32> qint32_4;

enum class QSSGRenderShaderDataType : quint32
{
    Unknown = 0,
    Integer, // qint32,
    IntegerVec2, // qint32_2,
    IntegerVec3, // qint32_3,
    IntegerVec4, // qint32_4,
    Boolean, // bool
    BooleanVec2, // bool_2,
    BooleanVec3, // bool_3,
    BooleanVec4, // bool_4,
    Float, // float,
    Vec2, // QVector2D,
    Vec3, // QVector3D,
    Vec4, // QVector4D,
    UnsignedInteger, // quint32,
    UnsignedIntegerVec2, // quint32_2,
    UnsignedIntegerVec3, // quint32_3,
    UnsignedIntegerVec4, // quint32_4,
    Matrix3x3, // QMatrix3x3,
    Matrix4x4, // QMatrix4x4,
    Texture2D, // QSSGRenderTexture2D *,
    Texture2DHandle, // QSSGRenderTexture2D **,
    Texture2DArray, // QSSGRenderTexture2DArray *,
    TextureCube, // QSSGRenderTextureCube *,
    TextureCubeHandle, // QSSGRenderTextureCube **,
    Image2D, // QSSGRenderImage2D *,
    DataBuffer // QSSGRenderDataBuffer *
};

template<typename TDataType>
struct QSSGDataTypeToShaderDataTypeMap
{
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<qint32>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Integer; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<qint32_2>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::IntegerVec2; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<qint32_3>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::IntegerVec3; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<qint32_4>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::IntegerVec4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<bool>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Boolean; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<bool_2>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::BooleanVec2; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<bool_3>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::BooleanVec3; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<bool_4>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::BooleanVec4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<float>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Float; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QVector2D>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Vec2; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QVector3D>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Vec3; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QVector4D>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Vec4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<quint32>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::UnsignedInteger; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<quint32_2>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::UnsignedIntegerVec2; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<quint32_3>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::UnsignedIntegerVec3; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<quint32_4>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::UnsignedIntegerVec4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QMatrix3x3>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Matrix3x3; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QMatrix4x4>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Matrix4x4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGDataView<QMatrix4x4>>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Matrix4x4; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderTexture2D *>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Texture2D; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderTexture2D **>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Texture2DHandle; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderTexture2DArray *>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Texture2DArray; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderTextureCube *>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::TextureCube; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderTextureCube **>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::TextureCubeHandle; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderImage2D *>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::Image2D; }
};

template<>
struct QSSGDataTypeToShaderDataTypeMap<QSSGRenderDataBuffer *>
{
    static QSSGRenderShaderDataType getType() { return QSSGRenderShaderDataType::DataBuffer; }
};

enum class QSSGRenderShaderTypeValue
{
    Unknown = 1 << 0,
    Vertex = 1 << 1,
    Fragment = 1 << 2,
    TessControl = 1 << 3,
    TessEvaluation = 1 << 4,
    Geometry = 1 << 5
};

Q_DECLARE_FLAGS(QSSGRenderShaderTypeFlags, QSSGRenderShaderTypeValue)
Q_DECLARE_OPERATORS_FOR_FLAGS(QSSGRenderShaderTypeFlags)

enum class QSSGRenderTextureTypeValue
{
    Unknown = 0,
    Diffuse,
    Specular,
    Environment,
    Bump,
    Normal,
    Displace,
    Emissive,
    Emissive2,
    Anisotropy,
    Translucent,
    LightmapIndirect,
    LightmapRadiosity,
    LightmapShadow
};

inline const char *toString(QSSGRenderTextureTypeValue value)
{
    switch (value) {
    case QSSGRenderTextureTypeValue::Unknown:
        return "Unknown";
    case QSSGRenderTextureTypeValue::Diffuse:
        return "Diffuse";
    case QSSGRenderTextureTypeValue::Specular:
        return "Specular";
    case QSSGRenderTextureTypeValue::Environment:
        return "Environment";
    case QSSGRenderTextureTypeValue::Bump:
        return "Bump";
    case QSSGRenderTextureTypeValue::Normal:
        return "Normal";
    case QSSGRenderTextureTypeValue::Displace:
        return "Displace";
    case QSSGRenderTextureTypeValue::Emissive:
        return "Emissive";
    case QSSGRenderTextureTypeValue::Emissive2:
        return "Emissive2";
    case QSSGRenderTextureTypeValue::Anisotropy:
        return "Anisotropy";
    case QSSGRenderTextureTypeValue::Translucent:
        return "Translucent";
    case QSSGRenderTextureTypeValue::LightmapIndirect:
        return "LightmapIndirect";
    case QSSGRenderTextureTypeValue::LightmapRadiosity:
        return "LightmapRadiosity";
    case QSSGRenderTextureTypeValue::LightmapShadow:
        return "LightmapShadow";
    }
    return nullptr;
}

enum class QSSGRenderReadPixelFormat
{
    Alpha8,
    RGB565,
    RGB8,
    RGBA4444,
    RGBA5551,
    RGBA8
};

inline int sizeofPixelFormat(QSSGRenderReadPixelFormat f)
{
    switch (f) {
    case QSSGRenderReadPixelFormat::Alpha8:
        return 1;
    case QSSGRenderReadPixelFormat::RGB565:
    case QSSGRenderReadPixelFormat::RGBA5551:
    case QSSGRenderReadPixelFormat::RGBA4444:
        return 2;
    case QSSGRenderReadPixelFormat::RGB8:
        return 3;
    case QSSGRenderReadPixelFormat::RGBA8:
        return 4;
    }

    Q_ASSERT(0);
    return 0;
}

// Now for scoped property access.
template<typename TBaseType, typename TDataType>
struct QSSGRenderGenericScopedProperty
{
    typedef void (TBaseType::*TSetter)(TDataType inType);
    typedef TDataType (TBaseType::*TGetter)() const;

    TBaseType &m_context;
    TSetter m_setter;
    TDataType m_initialValue;
    QSSGRenderGenericScopedProperty(TBaseType &ctx, TGetter getter, TSetter setter)
        : m_context(ctx), m_setter(setter), m_initialValue(((ctx).*getter)())
    {
    }
    QSSGRenderGenericScopedProperty(TBaseType &ctx, TGetter getter, TSetter setter, const TDataType &inNewValue)
        : m_context(ctx), m_setter(setter), m_initialValue(((ctx).*getter)())
    {
        ((m_context).*m_setter)(inNewValue);
    }
    ~QSSGRenderGenericScopedProperty() { ((m_context).*m_setter)(m_initialValue); }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(bool_2)
Q_DECLARE_METATYPE(bool_3)
Q_DECLARE_METATYPE(bool_4)
Q_DECLARE_METATYPE(quint32_2)
Q_DECLARE_METATYPE(quint32_3)
Q_DECLARE_METATYPE(quint32_4)
Q_DECLARE_METATYPE(qint32_2)
Q_DECLARE_METATYPE(qint32_3)
Q_DECLARE_METATYPE(qint32_4)

#endif
