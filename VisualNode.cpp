#include "VisualNode.h"
using namespace VisNodeSys;

Node::Node(const std::string ID)
{
	this->ID = ID;
	if (ID.empty())
		this->ID = FocalEngine::APPLICATION.GetUniqueHexID();

	SetSize(ImVec2(200, 80));
	SetName("VisualNode");
	Type = "VisualNode";
}

Node::Node(const Node& Src)
{
	ParentArea = Src.ParentArea;
	ID = FocalEngine::APPLICATION.GetUniqueHexID();
	Position = Src.Position;
	Size = Src.Size;

	ClientRegionMin = Src.ClientRegionMin;
	ClientRegionMax = Src.ClientRegionMax;

	Name = Src.Name;
	Type = Src.Type;
	Style = Src.Style;
	bShouldBeDestroyed = false;

	LeftTop = Src.LeftTop;
	RightBottom = Src.RightBottom;

	TitleBackgroundColor = Src.TitleBackgroundColor;
	TitleBackgroundColorHovered = Src.TitleBackgroundColorHovered;

	for (size_t i = 0; i < Src.Input.size(); i++)
	{
		Input.push_back(new NodeSocket(this, Src.Input[i]->GetType(), Src.Input[i]->GetName(), false, Src.Input[i]->OutputData));
	}

	for (size_t i = 0; i < Src.Output.size(); i++)
	{
		Output.push_back(new NodeSocket(this, Src.Output[i]->GetType(), Src.Output[i]->GetName(), true, Src.Output[i]->OutputData));
	}
}

Node::~Node()
{
	for (int i = 0; i < static_cast<int>(Input.size()); i++)
	{
		delete Input[i];
		Input.erase(Input.begin() + i, Input.begin() + i + 1);
		i--;
	}

	for (int i = 0; i < static_cast<int>(Output.size()); i++)
	{
		delete Output[i];
		Output.erase(Output.begin() + i, Output.begin() + i + 1);
		i--;
	}
}

std::string Node::GetID()
{
	return ID;
}

ImVec2 Node::GetPosition() const
{
	return Position;
}

void Node::SetPosition(const ImVec2 NewValue)
{
	Position = NewValue;
}

ImVec2 Node::GetSize() const
{
	if (GetStyle() == CIRCLE)
		return ImVec2(NODE_DIAMETER, NODE_DIAMETER);
	
	return Size;
}

void Node::SetSize(const ImVec2 NewValue)
{
	Size = NewValue;
}

std::string Node::GetName()
{
	return Name;
}

void Node::SetName(const std::string NewValue)
{
	if (NewValue.size() > NODE_NAME_MAX_LENGHT)
		return;

	Name = NewValue;
}

void Node::AddSocket(NodeSocket* Socket)
{
	if (Socket == nullptr)
		return;

	if (Socket->bOutput)
		Output.push_back(Socket);
	else
		Input.push_back(Socket);
}

void Node::Draw()
{
}

void Node::SocketEvent(NodeSocket* OwnSocket, NodeSocket* ConnectedSocket, NODE_SOCKET_EVENT EventType)
{

}

bool Node::CanConnect(NodeSocket* OwnSocket, NodeSocket* CandidateSocket, char** MsgToUser)
{
	// Socket can't connect to itself.
	if (OwnSocket == CandidateSocket)
		return false;

	// Nodes can't connect to themselves.
	if (CandidateSocket->GetParent() == this)
		return false;

	// Output can't connect to output and input can't connect to input.
	if (OwnSocket->bOutput == CandidateSocket->bOutput)
		return false;

	// Types must match.
	if (OwnSocket->GetType() != CandidateSocket->GetType())
		return false;

	return true;
}

std::string Node::GetType() const
{
	return Type;
}

Json::Value Node::ToJson()
{
	Json::Value result;

	result["ID"] = ID;
	result["nodeType"] = Type;
	result["nodeStyle"] = Style;
	result["position"]["x"] = Position.x;
	result["position"]["y"] = Position.y;
	result["size"]["x"] = Size.x;
	result["size"]["y"] = Size.y;
	result["name"] = Name;

	for (size_t i = 0; i < Input.size(); i++)
	{
		result["input"][std::to_string(i)]["ID"] = Input[i]->GetID();
		result["input"][std::to_string(i)]["name"] = Input[i]->GetName();
		result["input"][std::to_string(i)]["type"] = Input[i]->GetType();
	}

	for (size_t i = 0; i < Output.size(); i++)
	{
		result["output"][std::to_string(i)]["ID"] = Output[i]->GetID();
		result["output"][std::to_string(i)]["name"] = Output[i]->GetName();
		result["output"][std::to_string(i)]["type"] = Output[i]->GetType();
	}

	return result;
}

void Node::FromJson(Json::Value Json)
{
	ID = Json["ID"].asCString();
	Type = Json["nodeType"].asCString();
	if (Json.isMember("nodeStyle"))
		Style = NODE_STYLE(Json["nodeStyle"].asInt());
	Position.x = Json["position"]["x"].asFloat();
	Position.y = Json["position"]["y"].asFloat();
	Size.x = Json["size"]["x"].asFloat();
	Size.y = Json["size"]["y"].asFloat();
	Name = Json["name"].asCString();

	const std::vector<Json::String> InputsList = Json["input"].getMemberNames();
	for (size_t i = 0; i < Input.size(); i++)
	{
		delete Input[i];
		Input[i] = nullptr;
	}
	Input.resize(InputsList.size());
	for (size_t i = 0; i < InputsList.size(); i++)
	{
		const std::string ID = Json["input"][std::to_string(i)]["ID"].asCString();
		const std::string name = Json["input"][std::to_string(i)]["name"].asCString();

		// This is a temporary solution for compatibility with old files.
		std::string type = "FLOAT";
		if (Json["input"][std::to_string(i)]["type"].type() == Json::stringValue)
			type = Json["input"][std::to_string(i)]["type"].asCString();

		Input[i] = new NodeSocket(this, type, name, false);
		Input[i]->ID = ID;
	}

	const std::vector<Json::String> OutputsList = Json["output"].getMemberNames();
	for (size_t i = 0; i < Output.size(); i++)
	{
		delete Output[i];
		Output[i] = nullptr;
	}
	Output.resize(OutputsList.size());
	for (size_t i = 0; i < OutputsList.size(); i++)
	{
		const std::string ID = Json["output"][std::to_string(i)]["ID"].asCString();
		const std::string name = Json["output"][std::to_string(i)]["name"].asCString();

		// This is a temporary solution for compatibility with old files.
		std::string type = "FLOAT";
		if (Json["output"][std::to_string(i)]["type"].type() == Json::stringValue)
			type = Json["output"][std::to_string(i)]["type"].asCString();

		Output[i] = new NodeSocket(this, type, name, true);
		Output[i]->ID = ID;
	}
}

void Node::UpdateClientRegion()
{
	float LongestInputSocketTextW = 0.0f;
	for (size_t i = 0; i < Input.size(); i++)
	{
		const ImVec2 TextSize = ImGui::CalcTextSize(Input[i]->GetName().c_str());
		if (TextSize.x > LongestInputSocketTextW)
			LongestInputSocketTextW = TextSize.x;
	}

	float LongestOutputSocketTextW = 0.0f;
	for (size_t i = 0; i < Output.size(); i++)
	{
		const ImVec2 TextSize = ImGui::CalcTextSize(Output[i]->GetName().c_str());
		if (TextSize.x > LongestOutputSocketTextW)
			LongestOutputSocketTextW = TextSize.x;
	}

	ClientRegionMin.x = LeftTop.x + NODE_SOCKET_SIZE * 5.0f + LongestInputSocketTextW + 2.0f;
	ClientRegionMax.x = RightBottom.x - NODE_SOCKET_SIZE * 5.0f - LongestOutputSocketTextW - 2.0f;

	ClientRegionMin.y = LeftTop.y + NODE_TITLE_HEIGHT + 2.0f;
	ClientRegionMax.y = RightBottom.y - 2.0f;
}

ImVec2 Node::GetClientRegionSize()
{
	UpdateClientRegion();
	return ClientRegionMax - ClientRegionMin;
}

ImVec2 Node::GetClientRegionPosition()
{
	UpdateClientRegion();
	return ClientRegionMin;
}

size_t Node::InputSocketCount() const
{
	return Input.size();
}

size_t Node::OutSocketCount() const
{
	return Output.size();
}

std::vector<Node*> Node::GetNodesConnectedToInput() const
{
	std::vector<Node*> result;
	for (size_t i = 0; i < Input.size(); i++)
	{
		for (size_t j = 0; j < Input[i]->ConnectedSockets.size(); j++)
		{
			result.push_back(Input[i]->ConnectedSockets[j]->GetParent());
		}
	}

	return result;
}

std::vector<Node*> Node::GetNodesConnectedToOutput() const
{
	std::vector<Node*> result;
	for (size_t i = 0; i < Output.size(); i++)
	{
		for (size_t j = 0; j < Output[i]->ConnectedSockets.size(); j++)
		{
			result.push_back(Output[i]->ConnectedSockets[j]->GetParent());
		}
	}

	return result;
}

bool Node::OpenContextMenu()
{
	return false;
}

NODE_STYLE Node::GetStyle() const
{
	return Style;
}

void Node::SetStyle(const NODE_STYLE NewValue)
{
	if (static_cast<int>(NewValue) < 0 || static_cast<int>(NewValue) >= 2)
		return;

	Style = NewValue;
}

bool Node::IsHovered() const
{
	return bHovered;
}

void Node::SetIsHovered(const bool NewValue)
{
	bHovered = NewValue;
}

bool Node::CouldBeMoved() const
{
	return bCouldBeMoved;
}

void Node::SetCouldBeMoved(bool NewValue)
{
	bCouldBeMoved = NewValue;
}

NodeArea* Node::GetParentArea() const
{
	return ParentArea;
}