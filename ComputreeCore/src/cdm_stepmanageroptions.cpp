#include "cdm_stepmanageroptions.h"

#include <QFile>
#include <QLocale>

#include "cdm_configfile.h"

CDM_StepManagerOptions::CDM_StepManagerOptions()
{
    _nJumpInFastDebugMode = 20;
    _autoSave = false;
    _autoSaveDirPath = ".";
    _autoClearMemory = false;
    _locale = QLocale(QLocale::English, QLocale::UnitedKingdom).name();
}

bool CDM_StepManagerOptions::enableAutoSave(QString dirPath)
{
    if(QFile::exists(dirPath))
    {
        _autoSave = true;
        _autoSaveDirPath = dirPath;
    }

    return _autoSave;
}

void CDM_StepManagerOptions::disableAutoSave()
{
    _autoSave = false;
}

void CDM_StepManagerOptions::setAutoClearResultFromMemory(bool enable)
{
    _autoClearMemory = enable;
}

bool CDM_StepManagerOptions::changeFastJumpInDebugMode(int nJump)
{
    if(nJump > 0)
    {
        _nJumpInFastDebugMode = nJump;

        return true;
    }

    return false;
}

bool CDM_StepManagerOptions::isAutoSaveEnable() const
{
    return _autoSave;
}

QString CDM_StepManagerOptions::autoSaveDirPath() const
{
    return _autoSaveDirPath;
}

bool CDM_StepManagerOptions::isAutoClearResultFromMemoryEnable() const
{
    return _autoClearMemory;
}

int CDM_StepManagerOptions::getFastJumpValueInDebugMode() const
{
    return _nJumpInFastDebugMode;
}

void CDM_StepManagerOptions::setQLocale(QString locale)
{
    _locale = locale;
}

QString CDM_StepManagerOptions::getQLocale()
{
    return _locale;
}


void CDM_StepManagerOptions::load()
{
    CONFIG_FILE->beginGroup("StepManagerOptions");

    _autoSaveDirPath = CONFIG_FILE->value("defaultSavePath", autoSaveDirPath()).toString();
    changeFastJumpInDebugMode(CONFIG_FILE->value("nJumpInFastDebugMode", getFastJumpValueInDebugMode()).toInt());
    setQLocale(CONFIG_FILE->value("locale", getQLocale()).toString());

    CONFIG_FILE->endGroup();
}

bool CDM_StepManagerOptions::save() const
{
    CONFIG_FILE->beginGroup("StepManagerOptions");

    CONFIG_FILE->setValue("defaultSavePath", autoSaveDirPath());
    CONFIG_FILE->setValue("nJumpInFastDebugMode", getFastJumpValueInDebugMode());
    CONFIG_FILE->setValue("locale", _locale);

    CONFIG_FILE->endGroup();

    return true;
}
