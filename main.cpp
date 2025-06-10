#include <ctime>
#include <iostream>
#include <flatbuffers/flatbuffers.h>
#include <proto_generated.h>

using namespace Protos;

template <typename T>
struct MessageHandler
{
	static void Handle(const T *msg)
	{
		std::cout << "Unknown message type." << std::endl;
	}
};

template <typename T>
void DispatchMessageContent(const Protos::MessageRoot *msg)
{
	using OffsetType = flatbuffers::Offset<T>;
	static_assert(flatbuffers::is_specialisation_of_Offset<OffsetType>::value, "Type must be an Offset specialization.");

	const T *content = msg->content_as<T>();
	if (content)
	{
		MessageHandler<T>::Handle(content);
	}
	else
	{
		std::cout << "Invalid content type." << std::endl;
	}
}

void HandleMessage(const uint8_t *buf, size_t size)
{
	flatbuffers::Verifier verifier(buf, size);
	if (!Protos::VerifyMessageRootBuffer(verifier))
	{
		std::cout << "Invalid message buffer." << std::endl;
		return;
	}

	const Protos::MessageRoot *msg = Protos::GetMessageRoot(buf);
	if (!msg)
	{
		std::cout << "Failed to get message root." << std::endl;
		return;
	}

	std::cout << "Received message at time: " << msg->time() << std::endl;

	switch (msg->content_type())
	{
	case Protos::MessageContent::TextMessage:
		DispatchMessageContent<Protos::TextMessage>(msg);
		break;
	case Protos::MessageContent::ImageMessage:
		DispatchMessageContent<Protos::ImageMessage>(msg);
		break;
	default:
		std::cout << "Unknown message type." << std::endl;
		break;
	}
}

int main()
{
	flatbuffers::FlatBufferBuilder builder(1024);

	auto imgMsg = CreateImageMessageDirect(builder, "https://example.com/image.png", 800, 600);
	auto msgRoot = CreateMessageRoot(builder, time(nullptr), MessageContent::ImageMessage, imgMsg.Union());
	builder.Finish(msgRoot);

	HandleMessage(builder.GetBufferPointer(), builder.GetSize());

	return 0;
}

template <>
struct MessageHandler<Protos::TextMessage>
{
	static void Handle(const Protos::TextMessage *msg)
	{
		std::cout << "Text Message: " << msg->text()->str() << std::endl;
	}
};

template <>
struct MessageHandler<Protos::ImageMessage>
{
	static void Handle(const Protos::ImageMessage *msg)
	{
		std::cout << "Image URL: " << msg->url()->str()
				  << ", Width: " << msg->width()
				  << ", Height: " << msg->height() << std::endl;
	}
};
