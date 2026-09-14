#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include <pages/editor/codenode.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>
#include <storage/manifest.hpp>

namespace pad {

void appendTitleToJsonNote(QJsonObject &json, Note *note);

void buildJsonTextNode(size_t index, TextNode *node, QJsonObject &jsonNode);

QJsonArray buildJsonForNoteNodes(Note *note);

QJsonObject buildJsonForNote(Note *note);

Note *buildNoteFromJson(const QJsonObject &json, const Manifest &manifest);

} // namespace pad
