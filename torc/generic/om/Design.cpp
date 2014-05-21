// TORC - Copyright 2010 University of Southern California. All Rights Reserved.

#ifndef HAVE_CONFIG_H
#include "torc/generic/config.h"
#endif

#include "torc/generic/om/Design.hpp"

namespace torc {

namespace generic {

DesignSharedPtr
Design::Factory::newDesignPtr( const std::string &inName,
    const RootSharedPtr &inRootPtr,
    const std::string &inCellRefName,
    const std::string &inLibraryRefName,
    const std::string &inOriginalName ) throw(Error) {
    try
    {
        DesignSharedPtr newDesign;
        create( newDesign );
        newDesign->setName( inName );
        newDesign->setParent( inRootPtr );
        inRootPtr->addDesign( newDesign );
        newDesign->setCellRefName( inCellRefName );
        newDesign->setLibraryRefName( inLibraryRefName );
        newDesign->setOriginalName( inOriginalName );
        return newDesign;
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }
}

void
Design::accept(BaseVisitor & inoutVisitor) throw(Error) {
    try
    {
        runVisitor( *this, inoutVisitor );
    }
    catch( Error &e )
    {
        e.setCurrentLocation( __FUNCTION__,
                __FILE__, __LINE__ );
        throw;
    }
}

Design::Design()
    :Nameable(),
    Commentable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<Root>(),
    SelfReferencing<Design>(),
    UserDataContainer(),
    StatusContainer(),
    mCellRefName(),
    mLibraryRefName() {
}

Design::Design( const std::string & inCellRefName,
                    const std::string &inLibraryRefName )
    :Nameable(),
    Commentable(),
    PropertyContainer(),
    Renamable(),
    Visitable(),
    ParentedObject<Root>(),
    SelfReferencing<Design>(),
    UserDataContainer(),
    StatusContainer(),
    mCellRefName( inCellRefName ),
    mLibraryRefName( inLibraryRefName ) {
}

Design::~Design() throw() {
}

void
Design::setCellRefName( const std::string & inCellRefName ) throw(){
    mCellRefName = inCellRefName;
}

void
Design::setLibraryRefName( const std::string & inLibraryRefName ) throw(){
    mLibraryRefName = inLibraryRefName;
}

} // namespace torc::generic

} // namespace torc
