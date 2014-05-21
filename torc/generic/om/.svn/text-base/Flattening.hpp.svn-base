#ifndef TORC_GENERIC_FLATTENING_HPP
#define TORC_GENERIC_FLATTENING_HPP

#include "torc/generic/util/Error.hpp"
#include "torc/generic/om/PointerTypes.hpp"

namespace torc {
namespace generic {

/**
 * Flatten a given instance.
 * Instance flattening is an operation where a given instance is replaced the internals of it's master in a given view. Instance flattening can be achieved on Single or Array Instances. However, it is not supported on the elements of an instance array.
 *
 * @param inoutParentView[inout] View to which modifications would be done
 * @param inInstance[in] The instance that need to be flattened. This will be removed aafter the operation.
 * @param inFactory[in] The object factory used for cloning components in the instance.
 *
 * @note When connections are made to individual elements of a vector net using for example, say, the portList construct, the bits of the vectors are appropriately broken up into individual scalar nets, and conections removed from the vector nets.
 */
void
flatten( const ViewSharedPtr &inoutParentView,
        const InstanceSharedPtr &inoutInstance,
        const ObjectFactorySharedPtr &inFactory ) throw(Error);

} //namespace generic
} //namespace torc

#endif //TORC_GENERIC_FLATTENING_HPP
