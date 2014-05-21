#ifndef TORC_GENERIC_VISITORAPPLIER_HPP
#define TORC_GENERIC_VISITORAPPLIER_HPP

namespace torc
{
namespace generic
{

/**
 * This is an utility functor class that can be used to apply
 * visitor to a list of genOM objects. The type _Tp should be
 * replaced by the visitor type.
 */

template<typename _Tp>
class VisitorApplier {
    public:
        VisitorApplier( _Tp &inVisitor )
            :mVisitor( inVisitor ) {
        }

        template<typename _Up>
        void
        operator()( const boost::shared_ptr<_Up> &inVisited
                            ) const throw( Error ) {
            try
            {
                inVisited->accept( mVisitor );
            }
            catch( Error &e )
            {
                e.setCurrentLocation( __FUNCTION__,
                                            __FILE__, __LINE__ );
                throw;
            }
        }
    private:
        _Tp &mVisitor;
};

} //namespace generic
} //namespace torc


#endif
