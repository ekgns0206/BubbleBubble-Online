#include "PBThread.h"
using namespace PB;

thread::thread(thread::PBThreadCallbackRoutine callbackFunction, void * param)
{
	this->_handle			= NULL;
	this->_callback			= callbackFunction;
	this->_parameter		= param;
}

thread::~thread()
{
	if(this->_handle != NULL)
		::CloseHandle(this->_handle);
}

//
// run
//	�����带 �����Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
//
bool thread::run()
{
	this->_handle			= ::CreateThread(NULL, 0, thread::DefaultThreadRoutine, this, 0, NULL);
	return this->_handle != NULL;
}

//
// parameter
//	������� ���޵� �Ķ���͸� ��´�.
//
// Parameters
//	����
//
// Return
//	���޵� �Ķ����
//
void * thread::parameter() const
{
	return this->_parameter;
}

//
// wait
//	�����尡 ���� �� ���� ����Ѵ�.
//
// Parameters
//	timeout		Ÿ�Ӿƿ�
//
// Return
//	������ true, ���н� false
//
bool thread::wait(int timeout)
{
	return thread::wait(*this, timeout);
}

thread::operator HANDLE() const
{
	return this->_handle;
}

//
// wait
//	�����尡 ���� �� ���� ����Ѵ�.
//
// Parameters
//	thread		����� ������
//	timeout		Ÿ�Ӿƿ�
//
// Return
//	������ true, ���н� false
//
bool thread::wait(const thread & thread, int timeout)
{
	return ::WaitForSingleObject(thread, timeout) != WAIT_FAILED;
}

//
// DefaultThreadRoutine
//	CreateThread �Լ��� ���� ���޵� ������ �Լ� ����
//
// Parameters
//	args		����
//
// Return
//	��
//
DWORD thread::DefaultThreadRoutine(void * args)
{
	thread*				self = (thread*)args;
	self->_callback(self);
	return 0;
}

DWORD PB::thread::StaticDefaultThreadRoutine(void * parameter)
{
	StaticThreadParam*		staticThreadParam	= (StaticThreadParam*)parameter;
	thread				thread(staticThreadParam->callbackRoutine, staticThreadParam->parameter);

	thread._callback(&thread);

	delete staticThreadParam;
	return 0;
}

void thread::run(PBThreadCallbackRoutine callbackRoutine, void * param)
{
	StaticThreadParam*		staticThreadParam	= new StaticThreadParam();
	staticThreadParam->callbackRoutine			= callbackRoutine;
	staticThreadParam->parameter				= param;
	CloseHandle(::CreateThread(NULL, 0, thread::StaticDefaultThreadRoutine, staticThreadParam, 0, NULL));
}

csection_item::csection_item()
{
	::InitializeCriticalSection(&this->_criticalSection);
}

csection_item::csection_item(const csection_item & right) : csection_item()
{
}

csection_item::~csection_item()
{
	DeleteCriticalSection(&this->_criticalSection);
}

//
// enter
//	�Ӱ迵���� �����Ѵ�.
//
// Parameters
//	����
//
// Return
//	����
//
void csection_item::enter()
{
	::EnterCriticalSection(&this->_criticalSection);
}

//
// leave
//	�Ӱ迵������ �������´�.
//
// Parameters
//	����
//
// Return
//	����
//
void csection_item::leave()
{
	::LeaveCriticalSection(&this->_criticalSection);
}


csection* csection::_instance = NULL;

csection::csection()
{
}

csection::~csection()
{
	for(std::unordered_map<std::string, csection_item*>::iterator i = this->_criticalSectionTable.begin(); i != this->_criticalSectionTable.end(); i++)
		delete i->second;
}

void csection::init()
{
	csection::_instance		= new csection();
}

void csection::release()
{
	delete csection::_instance;
}

//
// enter
//	�Ӱ迵���� �����Ѵ�.
//
// Parameters
//	id		ũ��Ƽ�ü��� id
//
// Return
//	����
//
void csection::enter(const char* format, ...)
{
	va_list					args;
	va_start(args, format);

	char*							id			= new char[_vscprintf(format, args) + 1];
	vsprintf(id, format, args);

	if (csection::_instance->_criticalSectionTable.find(id) == csection::_instance->_criticalSectionTable.end())
		csection::_instance->_criticalSectionTable.insert(std::make_pair(std::string(id), new csection_item()));
	
	csection::_instance->_criticalSectionTable.at(id)->enter();

	delete[] id;
	va_end(args);
}

//
// leave
//	�Ӱ迵������ �������´�.
//
// Parameters
//	id		ũ��Ƽ�ü��� id
//
// Return
//	����
//
bool csection::leave(const char* format, ...)
{
	va_list					args;
	va_start(args, format);

	char*					id			= new char[_vscprintf(format, args) + 1];
	vsprintf(id, format, args);

	std::unordered_map<std::string, csection_item*>::iterator i = csection::_instance->_criticalSectionTable.find(id);
	if (i == csection::_instance->_criticalSectionTable.end())
		return false;

	i->second->leave();

	delete[] id;
	va_end(args);
	return true;
}