#include "behavior.h"


Node *Node::create(const std::string &name)
{
	Node *node = NULL;
	if (name == "and")
	{
		 node = new AndNode();
	}
	if (name == "or")
	{
		node = new OrNode();
	}
	if (node) node->name = name;
	return node;
}
void Node::addChild(Node *child)
{
	if (!this->child)
	{
		this->child = child;
	}
	else
	{
		Node *temp = this->child->next;
		Node * parent = this->child;
		while (temp)
		{
			if (!temp->next)
				parent = temp;
			temp = temp->next;
		}
		if (parent && child != this->child)
		{
			parent->next = child;
		}
	}
	child->parent = this;
}
LeafNode *JSONDocument::getLeafAction(const std::string &name)
{
	LEAF_NODES_ITER iter = leafNodes.find(name);
	if (iter != leafNodes.end())
	{
		iter->second->name = name;
		return iter->second->clone();
	}
	return NULL;
}
/**
 * {
 *    "name":"and",
 *	   "childs":[
 *	   		{
 *				"name":"sayHello",
 *			},
 *			{
 *				"name":"or",
 *				"childs":[{"name":"sayHello","content":" hello,world!"},]
 *			}
 * 	   ]	
 * }
 */
void JSONDocument::parseString(const char *content)
{
	JSONObject *object = new JSONObject(content); // ����JSONObject �Ķ���
	if (!object->isValid()) return;
	std::vector<JSONObject*> nodes;
	root = Node::create(object->get("name"));
	if (!root) return;
	object->extData = root;
	nodes.push_back(object);
	while (nodes.size()) // ����JSON �� ��ȡִ�нڵ�
	{
		JSONObject& temp = *nodes.back();
		JSONObject *preDel = &temp;
		Node *parent = temp.getExt<Node>(); // ��ȡ��չ����
		nodes.pop_back();
		JSONObject *object = new JSONObject(temp.getChild("childs").getChild()); // ��ȡ��һ���ӽڵ�
		while (object->isValid())
		{
			std::string name = object->get("name");
			Node *node = getLeafAction(name); // ��ȡ��ǰ�ӽڵ�
			JSONObject *preDel = NULL;
			if (!node) // ���Ǹ��ڵ�Ļ� 
			{
				node = Node::create(name);
				if (node)
				{
					object->extData = node;
					nodes.push_back(object); // ����Ԥ�����ݼ���
				}
			}
			else
			{
				node->name = name;
				LeafNode *leafNode = (LeafNode*) node;
				// ����Leaf������
				JSONObject attr = object->getChild().getNext();
				while (attr.isValid())
				{
					leafNode->attrs[attr.getName()] = attr.getText();
					attr = attr.getNext();
				}
				preDel = object;
			}
			parent->addChild(node); // �����ӽڵ�
			object = new JSONObject(object->getNext()); // ��ȡ��һ���ڵ�
		}
	}
}
/**
 * ��root �ع�Ϊstring
 */
std::string JSONDocument::toString()
{
	// ��root �ع�ΪNode�ַ���
#ifdef _NODE_DEBUG
	show(0,0);
#endif
	return createString(root).get();
}
double LeafNode::getDouble(const std::string &name)
{
	ATTRS_ITER iter = attrs.find(name);
	if (iter != attrs.end())
	{
		return atof(iter->second.c_str());
	}
	return 0;
}
int LeafNode::getInt(const std::string &name)
{
	ATTRS_ITER iter = attrs.find(name);
	if (iter != attrs.end())
	{
		return atoi(iter->second.c_str());
	}
	return 0;
}
std::string LeafNode::getString(const std::string &name)
{
	ATTRS_ITER iter = attrs.find(name);
	if (iter != attrs.end())
	{
		return iter->second.c_str();
	}
	return "";
}
JSONObject JSONDocument::createString(Node *root)
{
		if (!root) return "";
		JSONObject object;
		if (root->nodeType == Node::LEAF_NODE)
		{
			// ����Ҷ�ӽڵ�
			object.add("name",root->name.c_str());
			LeafNode *leafNode =(LeafNode*) root;
			for (LeafNode::ATTRS_ITER iter = leafNode->attrs.begin(); iter != leafNode->attrs.end();++iter)
			{
				object.add(iter->first.c_str(),iter->second.c_str());
			}
		}
		else
		{
			// ���ڵ�
			Node *child = root->child;
			if (root->nodeType == Node::AND_NODE)
			{
				object.add("name","and");
			}
			else
			{
				object.add("name","or");
			}
			JSONObject childs = JSONObject(cJSON_CreateArray());
			while (child)
			{
				JSONObject element = createString(child);
				childs.push(&element);
				child = child->next;
			}
			object.add("childs",&childs);
		}
		return object;
	}