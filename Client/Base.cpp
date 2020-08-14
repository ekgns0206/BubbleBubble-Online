#include "Base.h"

using namespace Scene;

Scene::Base::Base(PB::Windows::sprite * sprite, PB::Windows::sprite* backgroundSprite, const std::string& backgroundName, int width, int height) : scene(sprite, width, height, backgroundSprite, backgroundName)
{
}

Scene::Base::Base(const std::string & filename, PB::Windows::sprite * sprite, PB::Windows::sprite* backgroundSprite, const std::string& backgroundName) : scene(filename, sprite, backgroundSprite, backgroundName)
{
}

Scene::Base::~Base()
{
}

//
// addEvent
//	�̺�Ʈ �ڵ鷯�� ����Ѵ�.
//
// Parameters
//	method				json�� �޼ҵ� �̸�
//	callbackRoutine		ȣ��� �̺�Ʈ �ڵ鷯
//
// Return
//	������ true, ���н� false(����� �׻� true)
//
bool Base::addEvent(const std::string & method, BBPacketRoutine callbackRoutine)
{
	this->_eventTable.insert(std::make_pair(method, callbackRoutine));
	return true;
}

//
// onReceive
//	�����κ��� �����͸� �������� �� ȣ��Ǵ� �޼ҵ�
//	����� �̺�Ʈ ���̺�� �����Ǵ� �ڵ鷯�� �����Ѵٸ� ȣ���Ѵ�.
//
// Parameters
//	socket				���� ����
//	json				���޹��� json ��ü
//
// Return
//	����
//
void Base::onReceive(tcp & socket, Json::Value & response)
{
	std::string				method		= response["method"].asString();
	puts(method.c_str());
	Json::Value				request;
	request["method"]					= method;

	try
	{
		// �̺�Ʈ ���̺�κ��� �����Ǵ� �ڵ鷯 �޼ҵ带 ã�´�.
		std::unordered_map<std::string, BBPacketRoutine>::iterator i = this->_eventTable.find(method);
		if(i == this->_eventTable.end())
			throw std::exception("Undefined 'method' field");

		
		request["success"]				= true;

		// ȣ���Ѵ�.
		if((this->*i->second)(socket, request, response) == false)
			return;

		// �ڵ鷯�κ��� ���� request ��ü�� ������ ������.
		socket.send(encodeJson(request));
	}
	catch(std::exception& e)
	{
		request["success"]				= false;
		request["error"]				= e.what();
		//socket.send(encodeJson(request));			// �� �κ� �ּ� Ǯ�� �� ��û �ɸ�
	}
}

void Base::onDestroy()
{
	this->_sound.stop();
}

std::vector<char> Base::encodeJson(const Json::Value & json)
{
	
	std::string			str = json.toStyledString();
	int					size = str.length();
	char*				sizeRef = (char*)&size;

	std::vector<char>	buffer;
	buffer.insert(buffer.end(), sizeRef, sizeRef + sizeof(int));
	buffer.insert(buffer.end(), str.begin(), str.end());
	return buffer;
}

bool Base::decodeJson(const std::vector<char>& bytes, Json::Value& json)
{
	std::string		str(bytes.begin(), bytes.end());
	Json::Reader	reader;

	if (reader.parse(str, json) == false)
		return false;

	return true;
}