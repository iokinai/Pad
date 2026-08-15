#pragma once

#include <QAbstractListModel>
#include <QObject>

namespace pad {

class EditorController : public QObject {
  Q_OBJECT

  QAbstractListModel *_note;

  Q_PROPERTY(QAbstractListModel *note READ note CONSTANT)
public:
  explicit EditorController(QObject *parent = nullptr);

  QAbstractListModel *note() const noexcept;
};

} // namespace pad
