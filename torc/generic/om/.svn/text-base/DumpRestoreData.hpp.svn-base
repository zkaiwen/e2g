#ifndef TORC_GENERIC_DUMP_RESTORE_DATA_HPP
#define TORC_GENERIC_DUMP_RESTORE_DATA_HPP

#include "torc/generic/om/PointerTypes.hpp"
#include "torc/generic/om/DumpRestoreConfig.hpp"

#ifdef GENOM_SERIALIZATION
#include <string>

#include <boost/shared_ptr.hpp>

namespace torc { namespace generic { class ObjectFactory; } }

namespace torc {
namespace generic {

class DumpRestoreData {
  public:
    inline std::string
    getDumpPath() const throw();

    inline ObjectFactorySharedPtr
    getFactory() const throw();

    inline bool
    getRestoreAllComponents() const throw();

    explicit DumpRestoreData(
			const std::string &inDumpPath = std::string(),
			ObjectFactorySharedPtr inFactory
						= ObjectFactorySharedPtr(),
			bool inRestoreAllComponents = false  );

    DumpRestoreData( const DumpRestoreData &inRhs );

    ~DumpRestoreData() throw();

    DumpRestoreData &
    operator = ( const DumpRestoreData &inRhs );

  private:
    std::string mDumpPath;
    ObjectFactorySharedPtr mFactory;
    bool mRestoreAllComponents;
};

inline std::string
DumpRestoreData::getDumpPath() const throw() {
    return mDumpPath;
}

inline ObjectFactorySharedPtr
DumpRestoreData::getFactory() const throw() {
	return mFactory;
}

inline bool
DumpRestoreData::getRestoreAllComponents() const throw() {
    return mRestoreAllComponents;
}

} // namespace torc
} // namespace generic
#endif //GENOM_SERIALIZATION

#endif //TORC_GENERIC_DUMP_RESTORE_DATA_HPP
