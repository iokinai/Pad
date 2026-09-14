#include <QString>
#include <language/language.hpp>
#include <language/languagemodel.hpp>

namespace pad {

LanguageModel::LanguageModel(QVector<Language *> &&languages, QObject *parent)
    : QAbstractListModel(parent), _languages(std::move(languages)) {
  for (Language *language : _languages) {
    language->setParent(this);
  }
}

int LanguageModel::rowCount(const QModelIndex &parent) const {
  return _languages.size();
}

QVariant LanguageModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return {};

  switch (role) {
  case LanguageRole:
    return QVariant::fromValue(_languages[index.row()]);
  case LanguageNameRole:
    return _languages[index.row()]->name();
  default:
    return {};
  }

  return {};
}

QHash<int, QByteArray> LanguageModel::roleNames() const {
  return {{LanguageRole, "language"}, {LanguageNameRole, "name"}};
}

} // namespace pad
