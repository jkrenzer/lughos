#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>

class connectionImpl
{
public:

// 	connectionImpl(void);
// 	~connectionImpl(void);

	virtual bool start()=0;
	virtual void stop()=0;
// 	virtual int write(const std::string &buf)=0;
// 	virtual int write_async(const std::string &buf)=0;
	virtual std::string inputoutput(const std::string input, const int async)=0;
	virtual void set_port()=0;
	virtual void reset()=0;
	

};

template <class C> class connectionTemplate : public connectionImpl
{
public:
  C context;
};

template <class C> class connection : public connectionTemplate<C>
{};
#endif