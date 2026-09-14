#pragma once

#include <QAbstractListModel>
#include <language/language.hpp>

namespace pad {

class LanguageModel : public QAbstractListModel {
  Q_OBJECT

  QVector<Language *> _languages;

public:
  LanguageModel(QVector<Language *> &&languages, QObject *parent = nullptr);

  enum Roles {
    LanguageRole = Qt::UserRole + 1,
    LanguageNameRole,
  };

  Q_ENUM(Roles)

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;
};

} // namespace pad
