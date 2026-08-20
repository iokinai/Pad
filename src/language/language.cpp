#include <language/language.hpp>

namespace pad {

Language::Language(const QString &name, const QString &localizationFile,
                   QObject *parent)
    : QObject(parent), _name(name), _localizationFile(localizationFile) {}

const QString &Language::name() const { return _name; }

const QString &Language::localizationFile() const { return _localizationFile; }

} // namespace pad
