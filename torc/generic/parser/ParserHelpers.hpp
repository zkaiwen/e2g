#ifndef TORC_GENERIC_PARSER_HELPERS_HPP
#define TORC_GENERIC_PARSER_HELPERS_HPP

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "torc/generic/om/Value.hpp"

namespace torc { namespace generic { class Parameter; } }
namespace torc { namespace generic { class PortAttributes; } }
namespace torc { namespace generic { class TimeStamp; } }
namespace torc { namespace generic { class Written; } }

namespace torc {
namespace generic {

struct NameData {
    std::string mName;
    std::string mOriginalName;
    std::vector<size_t> mIndices;

    ~NameData() throw()
    {
    }
};

struct ArrayData {
    NameData *mNameData;
    std::vector<size_t> mIndices;
};

struct ViewRefData {
    NameData *mViewName;
    NameData *mCellName;
    NameData *mLibraryName;

    ViewRefData()
        :mViewName( NULL ),
        mCellName( NULL ),
        mLibraryName( NULL ) {
    }

    ~ViewRefData() {
        delete mViewName;
        delete mCellName;
        delete mLibraryName;
    }
};

struct CellRefData {
    NameData *mCellName;
    NameData *mLibraryName;

    CellRefData()
        :mCellName( NULL ),
        mLibraryName( NULL ) {
    }

    ~CellRefData() {
        delete mCellName;
        delete mLibraryName;
    }
};

struct InstanceRefData {
    NameData *mName;
    ViewRefData *mView;
    InstanceRefData()
        :mName( NULL ),
        mView( NULL ) {
    }
};

struct PortRefData {
    PortRefData *mParentPort;
    NameData *mPortName;
    NameData *mInstanceName;
    ViewRefData *mView;

    PortRefData()
        :mParentPort( NULL ),
        mPortName( NULL ),
        mInstanceName( NULL ),
           mView( NULL ) {
    }

    ~PortRefData()
    {
        delete mParentPort;
        delete mPortName;
        delete mInstanceName;
        delete mView;
    }
};

struct PortInstData {
    PortRefData *mRefData;
    PortAttributes *mAttribute;

    PortInstData()
        :mRefData( NULL ),
        mAttribute( NULL ) {
    }

    ~PortInstData() {
        delete mRefData;
    }
};

struct PortListData {
    std::vector<PortRefData *> mPortRefs;

    PortListData()
        :mPortRefs() {
    }

    ~PortListData() {
        for( std::vector<PortRefData *>::iterator it
                                            = mPortRefs.begin();
                it != mPortRefs.end(); ++it )
        {
            delete *it;
        }
    }
};

struct VersionData
{
    int32_t mFirst;
    int32_t mSecond;
    int32_t mThird;
};

struct ValueData
{
    std::vector<Value> mValues;
};

struct PairData
{
    int32_t mX;
    int32_t mY;

    PairData( int32_t inX = 0, int32_t inY = 0 )
        : mX( inX ),
        mY( inY ) {
    }
};

struct PairStrData {
    std::string mFirst;
    std::string mSecond;
    
    PairStrData( std::string inFirst = std::string(), 
                    std::string inSecond = std::string())
        :mFirst( inFirst ), 
        mSecond( inSecond ) { 
    }
};

struct ParamAssignInfo
{
    NameData *mNameData;
    ValueData *mValueData;

    ParamAssignInfo( NameData *inNameData = NULL,
            ValueData *inValueData = NULL )
        :mNameData( inNameData ),
        mValueData( inValueData ) {
    }
    ~ParamAssignInfo() {
        delete mNameData;
        delete mValueData;
    }
};

struct InstanceInfo {
    ViewRefData *mViewRefData;
    std::vector<ParamAssignInfo *> mParamAssign;
    std::vector<PortInstData *> mPortInst;
    std::vector< std::string > mUserData;
    std::vector< std::string > mComment;

    InstanceInfo()
        :mViewRefData(NULL),
         mParamAssign(),
         mPortInst(),
         mUserData(),
         mComment() {
    }

    ~InstanceInfo() {
        delete mViewRefData;
        for( std::vector<ParamAssignInfo *>::iterator
                it = mParamAssign.begin();
                it != mParamAssign.end(); it++ )
        {
            delete (*it);
        }
        for( std::vector<PortInstData *>::iterator
                it = mPortInst.begin();
                it != mPortInst.end(); it++ )
        {
            delete (*it);
        }        
    }
};

} //namespace generic 
} //namespace torc

#endif //TORC_GENERIC_PARSER_HELPERS_HPP
