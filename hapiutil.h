#ifndef __hapiutil_h__
#define __hapiutil_h__

#include <HAPI/HAPI.h>

#include "traits.h"
#include "util.h"

template <typename T>
struct HAPITypeTrait
{
};

template <>
struct HAPITypeTrait<int>
{
    static const HAPI_StorageType storageType = HAPI_STORAGETYPE_INT;
};

template <>
struct HAPITypeTrait<float>
{
    static const HAPI_StorageType storageType = HAPI_STORAGETYPE_FLOAT;
};

template <>
struct HAPITypeTrait<const char*>
{
    static const HAPI_StorageType storageType = HAPI_STORAGETYPE_STRING;
};

template <>
struct HAPITypeTrait<std::string>
{
    static const HAPI_StorageType storageType = HAPI_STORAGETYPE_STRING;
};

template <>
struct HAPITypeTrait<MString>
{
    static const HAPI_StorageType storageType = HAPI_STORAGETYPE_STRING;
};

template<HAPI_StorageType storageType>
struct HAPIAttributeTrait
{
    typedef void SetType;
    typedef void GetType;

    //static const HAPI_StorageType storageType = HAPI_STORAGETYPE_INVALID;

    static HAPI_Result setAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            const char* name,
            const HAPI_AttributeInfo* attrInfo,
            const SetType* dataArray,
            int start, int length
            ) { return HAPI_RESULT_FAILURE; }

    static HAPI_Result getAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            const char* name,
            HAPI_AttributeInfo* attrInfo,
            GetType* dataArray,
            int start, int length
            ) { return HAPI_RESULT_FAILURE; }
};

template<>
struct HAPIAttributeTrait<HAPI_STORAGETYPE_INT>
{
    typedef int SetType;
    typedef int GetType;

    //static const HAPI_StorageType storageType = HAPI_STORAGETYPE_INT;

    static HAPI_Result setAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            const char* name,
            const HAPI_AttributeInfo* attrInfo,
            const SetType* dataArray,
            int start, int length
            )
    {
        return HAPI_SetAttributeIntData(
                session,
                assetId, objectId, geoId,
                name,
                attrInfo,
                dataArray,
                start, length
                );
    }

    static HAPI_Result getAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            const char* name,
            HAPI_AttributeInfo* attrInfo,
            GetType* dataArray,
            int start, int length
            )
    {
        return HAPI_GetAttributeIntData(
                session,
                assetId, objectId, geoId, partId,
                name,
                attrInfo,
                dataArray,
                start, length
                );
    }
};

template<>
struct HAPIAttributeTrait<HAPI_STORAGETYPE_FLOAT>
{
    typedef float SetType;
    typedef float GetType;

    //static const HAPI_StorageType storageType = HAPI_STORAGETYPE_FLOAT;

    static HAPI_Result setAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            const char* name,
            const HAPI_AttributeInfo* attrInfo,
            const SetType* dataArray,
            int start, int length
            )
    {
        return HAPI_SetAttributeFloatData(
                session,
                assetId, objectId, geoId,
                name,
                attrInfo,
                dataArray,
                start, length
                );
    }

    static HAPI_Result getAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            const char* name,
            HAPI_AttributeInfo* attrInfo,
            GetType* dataArray,
            int start, int length
            )
    {
        return HAPI_GetAttributeFloatData(
                session,
                assetId, objectId, geoId, partId,
                name,
                attrInfo,
                dataArray,
                start, length
                );
    }
};

template<>
struct HAPIAttributeTrait<HAPI_STORAGETYPE_STRING>
{
    typedef const char* SetType;
    typedef int GetType;

    //static const HAPI_StorageType storageType = HAPI_STORAGETYPE_STRING;

    static HAPI_Result setAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            const char* name,
            const HAPI_AttributeInfo* attrInfo,
            const SetType* dataArray,
            int start, int length
            )
    {
        return HAPI_SetAttributeStringData(
                session,
                assetId, objectId, geoId,
                name,
                attrInfo,
                (const char**) dataArray,
                start, length
                );
    }

    static HAPI_Result getAttribute(
            const HAPI_Session* session,
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            const char* name,
            HAPI_AttributeInfo* attrInfo,
            GetType* dataArray,
            int start, int length
            )
    {
        return HAPI_GetAttributeStringData(
                session,
                assetId, objectId, geoId, partId,
                name,
                attrInfo,
                dataArray,
                start, length
                );
    }
};

template<typename T, typename U>
struct SameType
{
    static const bool value = false;
};

template<typename T>
struct SameType<T, T>
{
    static const bool value = true;
};

template<
    HAPI_StorageType storageType,
    typename T,
    bool canUseData
        = ArrayTrait<T>::canGetData
        && SameType<
            typename ArrayTrait<T>::ElementType,
            typename HAPIAttributeTrait<
                HAPITypeTrait<
                    typename ArrayTrait<T>::ElementType
                    >::storageType>::SetType
            >::value
    >
struct HAPISetAttribute
{
    static HAPI_Result impl(
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_AttributeOwner owner,
            size_t tupleSize,
            const char* attributeName,
            const T &dataArray
            )
    {
        HAPI_Result hapiResult;

        if(tupleSize == 0)
            return HAPI_RESULT_SUCCESS;

        size_t dataArraySize = ArrayTrait<T>::size(dataArray);

        size_t count = dataArraySize / tupleSize;

        if(count == 0)
            return HAPI_RESULT_SUCCESS;

        HAPI_AttributeInfo attributeInfo;
        attributeInfo.exists = true;
        attributeInfo.owner = owner;
        attributeInfo.storage = storageType;
        attributeInfo.count = count;
        attributeInfo.tupleSize = tupleSize;

        hapiResult = HAPI_AddAttribute(
                Util::theHAPISession.get(),
                assetId, objectId, geoId,
                attributeName,
                &attributeInfo
                );
        CHECK_HAPI_AND_RETURN(hapiResult, hapiResult);

        hapiResult = HAPIAttributeTrait<storageType>::setAttribute(
                    Util::theHAPISession.get(),
                    assetId, objectId, geoId,
                    attributeName,
                    &attributeInfo,
                    ArrayTrait<T>::data(dataArray),
                    0, count
                    );
        CHECK_HAPI_AND_RETURN(hapiResult, hapiResult);

        return HAPI_RESULT_SUCCESS;
    }
};

template<HAPI_StorageType storageType, typename T>
struct HAPISetAttribute<storageType, T, false>
{
    static HAPI_Result impl(
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_AttributeOwner owner,
            size_t tupleSize,
            const char* attributeName,
            const T &dataArray
            )
    {
        typedef typename HAPIAttributeTrait<storageType>::SetType SetType;
        typedef std::vector<SetType> ConvertedDataArray;

        ConvertedDataArray convertedDataArray
            = Util::convertArray<ConvertedDataArray>(dataArray);

        return HAPISetAttribute<storageType, ConvertedDataArray>::impl(
                assetId, objectId, geoId,
                owner,
                tupleSize,
                attributeName,
                convertedDataArray
                );
    }
};

template<typename T>
HAPI_Result
hapiSetAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_AttributeOwner owner,
        size_t tupleSize,
        const char* attributeName,
        const T &dataArray
        )
{
    return HAPISetAttribute<
        HAPITypeTrait<typename ArrayTrait<T>::ElementType>::storageType,
        T
        >::impl(
            assetId, objectId, geoId,
            owner,
            tupleSize,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiSetDetailAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        const char* attributeName,
        const T &dataArray
        )
{
    return hapiSetAttribute(
            assetId, objectId, geoId,
            HAPI_ATTROWNER_DETAIL,
            ArrayTrait<T>::size(dataArray),
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiSetPrimAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        size_t tupleSize,
        const char* attributeName,
        const T &dataArray
        )
{
    return hapiSetAttribute(
            assetId, objectId, geoId,
            HAPI_ATTROWNER_PRIM,
            tupleSize,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiSetVertexAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        size_t tupleSize,
        const char* attributeName,
        const T &dataArray
        )
{
    return hapiSetAttribute(
            assetId, objectId, geoId,
            HAPI_ATTROWNER_VERTEX,
            tupleSize,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiSetPointAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        size_t tupleSize,
        const char* attributeName,
        const T &dataArray
        )
{
    return hapiSetAttribute(
            assetId, objectId, geoId,
            HAPI_ATTROWNER_POINT,
            tupleSize,
            attributeName,
            dataArray
            );
}

template<
    HAPI_StorageType storageType,
    typename T,
    bool canUseData
        = ArrayTrait<T>::canGetData
        && SameType<
            typename ArrayTrait<T>::ElementType,
            typename HAPIAttributeTrait<
                HAPITypeTrait<
                    typename ArrayTrait<T>::ElementType
                    >::storageType>::GetType
            >::value
    >
struct HAPIGetAttribute
{
    static HAPI_Result impl(
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            HAPI_AttributeOwner owner,
            const char* attributeName,
            T &dataArray
            )
    {
        HAPI_Result hapiResult;

        HAPI_AttributeInfo attrInfo = HAPI_AttributeInfo_Create();

        hapiResult = HAPI_GetAttributeInfo(
                Util::theHAPISession.get(),
                assetId, objectId, geoId, partId,
                attributeName,
                owner,
                &attrInfo
                );
        CHECK_HAPI_AND_RETURN(hapiResult, hapiResult);

        if(!attrInfo.exists)
        {
            ArrayTrait<T>::resize(dataArray, 0);
            return HAPI_RESULT_FAILURE;
        }


        if(attrInfo.storage != storageType)
        {
            return HAPI_RESULT_FAILURE;
        }

        ArrayTrait<T>::resize(dataArray, attrInfo.count * attrInfo.tupleSize);
        hapiResult = HAPIAttributeTrait<storageType>::getAttribute(
                Util::theHAPISession.get(),
                assetId, objectId, geoId, partId,
                attributeName,
                &attrInfo,
                ArrayTrait<T>::data(dataArray),
                0, attrInfo.count
                );
        CHECK_HAPI_AND_RETURN(hapiResult, hapiResult);

        return HAPI_RESULT_SUCCESS;
    }
};

template<HAPI_StorageType storageType, typename T>
struct HAPIGetAttribute<storageType, T, false>
{
    static HAPI_Result impl(
            HAPI_AssetId assetId,
            HAPI_ObjectId objectId,
            HAPI_GeoId geoId,
            HAPI_PartId partId,
            HAPI_AttributeOwner owner,
            const char* attributeName,
            T &dataArray
            )
    {
        typedef typename HAPIAttributeTrait<storageType>::GetType GetType;
        typedef std::vector<GetType> ConvertedDataArray;

        HAPI_Result hapiResult;

        ConvertedDataArray convertedDataArray;

        hapiResult = HAPIGetAttribute<storageType, ConvertedDataArray>::impl(
                assetId, objectId, geoId, partId,
                owner,
                attributeName,
                convertedDataArray
                );
        CHECK_HAPI_AND_RETURN(hapiResult, hapiResult);

        dataArray = Util::convertArray<T>(convertedDataArray);

        return HAPI_RESULT_SUCCESS;
    }
};

template<typename T>
HAPI_Result
hapiGetAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_PartId partId,
        HAPI_AttributeOwner owner,
        const char* attributeName,
        T &dataArray
        )
{
    return HAPIGetAttribute<
        HAPITypeTrait<typename ArrayTrait<T>::ElementType>::storageType,
        T
        >::impl(
            assetId, objectId, geoId, partId,
            owner,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiGetDetailAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_PartId partId,
        const char* attributeName,
        T &dataArray
        )
{
    return hapiGetAttribute(
            assetId, objectId, geoId, partId,
            HAPI_ATTROWNER_DETAIL,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiGetPrimAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_PartId partId,
        const char* attributeName,
        T &dataArray
        )
{
    return hapiGetAttribute(
            assetId, objectId, geoId, partId,
            HAPI_ATTROWNER_PRIM,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiGetVertexAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_PartId partId,
        const char* attributeName,
        T &dataArray
        )
{
    return hapiGetAttribute(
            assetId, objectId, geoId, partId,
            HAPI_ATTROWNER_VERTEX,
            attributeName,
            dataArray
            );
}

template<typename T>
HAPI_Result
hapiGetPointAttribute(
        HAPI_AssetId assetId,
        HAPI_ObjectId objectId,
        HAPI_GeoId geoId,
        HAPI_PartId partId,
        const char* attributeName,
        T &dataArray
        )
{
    return hapiGetAttribute(
            assetId, objectId, geoId, partId,
            HAPI_ATTROWNER_POINT,
            attributeName,
            dataArray
            );
}

#endif