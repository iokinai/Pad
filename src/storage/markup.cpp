#include <exceptions/invalidjson.hpp>
#include <exceptions/invalidjsonnodetype.hpp>
#include <pages/editor/codenode.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/textnode.hpp>
#include <storage/manifest.hpp>
#include <storage/markup.hpp>

namespace pad {

enum class ValidationType { Array, String, Object, Number };

void appendTitleToJsonNote(QJsonObject &json, Note *note) {
  json["title"] = note->title();
}

void buildJsonTextNode(size_t index, TextNode *node, QJsonObject &jsonNode) {
  jsonNode["index"] = static_cast<int>(index);
  jsonNode["text"] = node->text();
  jsonNode["type"] = "text";
}

void buildJsonImageNode(size_t index, ImageNode *node, QJsonObject &jsonNode) {
  jsonNode["index"] = static_cast<int>(index);
  jsonNode["path"] = node->src();
  jsonNode["type"] = "image";
}

void buildJsonCodeNode(size_t index, CodeNode *node, QJsonObject &jsonNode) {
  jsonNode["index"] = static_cast<int>(index);
  jsonNode["code"] = node->code();
  jsonNode["type"] = "code";
}

QJsonArray buildJsonForNoteNodes(Note *note) {
  QJsonArray result;

  for (size_t i = 0; i < note->nodes().size(); ++i) {
    auto &node = note->nodes()[i];
    QJsonObject jsonNode;

    switch (node->type()) {
    case Node::Text: {
      auto textNode = dynamic_cast<TextNode *>(node);
      buildJsonTextNode(i, textNode, jsonNode);
      break;
    }
    case Node::Image: {
      auto imageNode = dynamic_cast<ImageNode *>(node);
      buildJsonImageNode(i, imageNode, jsonNode);
      break;
    }
    case Node::Code: {
      auto codeNode = dynamic_cast<CodeNode *>(node);
      buildJsonCodeNode(i, codeNode, jsonNode);
      break;
    }
    }

    result.append(jsonNode);
  }
  return result;
}

QJsonObject buildJsonForNote(Note *note) {
  QJsonObject result;

  appendTitleToJsonNote(result, note);
  result["nodes"] = buildJsonForNoteNodes(note);

  return result;
}

static void validateObjectType(const QJsonValue &json, ValidationType type) {
  switch (type) {
  case ValidationType::Array:
    if (!json.isArray()) {
      throw InvalidJson("Expected an array, got a different type");
    }
    break;
  case ValidationType::String:
    if (!json.isString()) {
      throw InvalidJson("Expected a string, got a different type");
    }
    break;
  case ValidationType::Object:
    if (!json.isObject()) {
      throw InvalidJson("Expected an object, got a different type");
    }
    break;
  case ValidationType::Number:
    if (!json.isDouble()) {
      throw InvalidJson("Expected a number, got a different type");
    }
    break;
  }
}

TextNode *buildTextNodeFromJson(const QJsonObject &nodeObject, Note *parent) {
  auto text = nodeObject["text"];
  validateObjectType(text, ValidationType::String);
  auto nodeText = text.toString();
  return new TextNode(nodeText, parent);
}

ImageNode *buildImageNodeFromJson(const QJsonObject &nodeObject, Note *parent) {
  auto path = nodeObject["path"];
  validateObjectType(path, ValidationType::String);
  auto nodePath = path.toString();
  return new ImageNode(nodePath, parent);
}

CodeNode *buildCodeNodeFromJson(const QJsonObject &nodeObject, Note *parent) {
  auto code = nodeObject["code"];
  validateObjectType(code, ValidationType::String);
  auto nodeCode = code.toString();
  return new CodeNode(nodeCode, parent);
}

QVector<Node *> buildNodesFromJson(const QJsonArray &jsonNodes, Note *parent) {
  QVector<Node *> preResult;

  for (const auto &jsonNode : jsonNodes) {
    Node *node = nullptr;

    validateObjectType(jsonNode, ValidationType::Object);
    auto nodeObject = jsonNode.toObject();

    auto type = nodeObject["type"];
    validateObjectType(type, ValidationType::String);
    auto nodeType = type.toString();
    auto index = nodeObject["index"];
    validateObjectType(index, ValidationType::Number);
    auto nodeIndex = index.toInt();

    if (nodeType == "text") {
      node = buildTextNodeFromJson(nodeObject, parent);
    } else if (nodeType == "image") {
      node = buildImageNodeFromJson(nodeObject, parent);
    } else if (nodeType == "code") {
      node = buildCodeNodeFromJson(nodeObject, parent);
    } else {
      throw InvalidJsonNodeType(nodeType);
    }

    preResult.insert(nodeIndex, node);
  }

  return preResult;
}

Note *buildNoteFromJson(const QJsonObject &json, const Manifest &manifest,
                        MediaStorage *storage) {
  auto jsonTitle = json["title"];
  validateObjectType(jsonTitle, ValidationType::String);
  auto jsonNodes = json["nodes"];
  validateObjectType(jsonNodes, ValidationType::Array);

  Note *note =
      new Note(jsonTitle.toString(), {}, manifest.createdAt(), storage);
  note->unsafeSetNodes(buildNodesFromJson(jsonNodes.toArray(), note));
  return note;
}

} // namespace pad
