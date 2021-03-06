#ifndef APPMODEL_H
#define APPMODEL_H

#include <QAbstractListModel>

#include "clickinstaller.h"

class QNetworkAccessManager;

class ApplicationItem: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QString appId READ appId CONSTANT)
    Q_PROPERTY(QString author READ author CONSTANT)
    Q_PROPERTY(QString tagline READ tagline CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString category READ category CONSTANT)
    Q_PROPERTY(QStringList screenshots READ screenshots CONSTANT)
    Q_PROPERTY(QString changelog READ changelog CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString installedVersion READ installedVersion NOTIFY installedChanged)
    Q_PROPERTY(QString packageUrl READ packageUrl CONSTANT)
    Q_PROPERTY(QString source READ source CONSTANT)
    Q_PROPERTY(QString license READ license CONSTANT)
    Q_PROPERTY(QString maintainer READ maintainer CONSTANT)
    Q_PROPERTY(int fileSize READ fileSize CONSTANT)
    Q_PROPERTY(bool installed READ installed NOTIFY installedChanged)
    Q_PROPERTY(int hooksCount READ hooksCount CONSTANT)


    Q_ENUMS(Hook)
    Q_FLAGS(Hooks)
public:
    enum Hook {
        HookNone = 0,
        HookDesktop = 1,
        HookScope = 2,
        HookAccountService = 4,
        HookUrls = 8,
        HookContentHub = 16,
        HookPushHelper = 32
    };
    Q_DECLARE_FLAGS(Hooks, Hook)

    struct HookStruct {
        QString name;
        Hooks hooks;
        QStringList permissions;
        QString apparmorTemplate;
        QStringList readPaths;
        QStringList writePaths;
    };

    explicit ApplicationItem(const QString &appId, QObject *parent = 0):
        QObject(parent),
        m_appId(appId)
    {}

    QString appId() const { return m_appId; }

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    QString author() const { return m_author; }
    void setAuthor(const QString &author) { m_author = author; }

    QString icon() const { return m_icon; }
    void setIcon(const QString &icon) { m_icon = icon; }

    QString tagline() const { return m_tagline; }
    void setTagline(const QString &tagline) { m_tagline = tagline; }

    QString description() const { return m_description; }
    void setDescription(const QString &description) { m_description = description; }

    QString category() const { return m_category; }
    void setCategory(const QString &category) { m_category = category; }

    QStringList screenshots() const { return m_screenshots; }
    void setScreenshots(const QStringList &screenshots) { m_screenshots = screenshots; }

    QString changelog() const { return m_changelog; }
    void setChangelog(const QString &changelog) { m_changelog = changelog; }

    QString version() const { return m_version; }
    void setVersion(const QString &version) { m_version = version; }

    QString installedVersion() const { return m_installedVersion; }
    void setInstalledVersion(const QString &version) { m_installedVersion = version; Q_EMIT installedChanged(); }

    QString packageUrl() const { return m_packageUrl; }
    void setPackageUrl(const QString &packageUrl) { m_packageUrl = packageUrl; }

    QString source() const { return m_source; }
    void setSource(const QString &source) { m_source = source; }

    QString license() const { return m_license; }
    void setLicense(const QString &license) { m_license = license; }

    QString maintainer() const { return m_maintainer; }
    void setMaintainer(const QString &maintainer) { m_maintainer = maintainer; }

    int fileSize() const { return m_fileSize; }
    void setFileSize(int fileSize) { m_fileSize = fileSize; }

    bool installed() const { return !m_installedVersion.isEmpty(); }

    Q_INVOKABLE QString permissions(int index) const { return m_hooks.at(index).permissions.join(", "); }
    Q_INVOKABLE Hooks hooks(int index) const { return m_hooks.at(index).hooks; }
    Q_INVOKABLE QString hookName(int index) { return m_hooks.at(index).name; }
    Q_INVOKABLE QString apparmorTemplate(int index) { return m_hooks.at(index).apparmorTemplate; }
    Q_INVOKABLE QString readPaths(int index) { return m_hooks.at(index).readPaths.join(", "); }
    Q_INVOKABLE QString writePaths(int index) { return m_hooks.at(index).writePaths.join(", "); }
    int hooksCount() const { return m_hooks.count(); }

    void setHooks(QList<HookStruct> hooksStruct) { m_hooks = hooksStruct; }

Q_SIGNALS:
    void installedChanged();

private:
    QString m_appId;
    QString m_name;
    QString m_icon;
    QString m_author;
    QString m_tagline;
    QString m_description;
    QString m_category;
    QStringList m_screenshots;
    QString m_changelog;
    QString m_packageUrl;
    QString m_version;
    QString m_source;
    QString m_license;
    QString m_maintainer;
    int m_fileSize;
    QString m_installedVersion;
    QList<HookStruct> m_hooks;

};

class AppModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ClickInstaller* installer READ installer  WRITE setInstaller NOTIFY installerChanged)

public:
    enum Roles {
        RoleName,
        RoleAppId,
        RoleIcon,
        RoleAuthor,
        RoleTagline,
        RoleDescription,
        RoleCategory,
        RoleScreenshots,
        RoleChangelog,
        RolePackageUrl,
        RoleVersion,
        RoleInstalled,
        RoleInstalledVersion,
        RoleUpdateAvailable,
        RoleMaintainer,
        RoleSearchHackishString
    };

    explicit AppModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int findApp(const QString &appId) const;
    Q_INVOKABLE ApplicationItem *app(int index) const;

    Q_INVOKABLE void install(int index);

    ClickInstaller* installer();
    void setInstaller(ClickInstaller* installer);

private Q_SLOTS:
    void loadAppList();

    void repoListFetched();

    void buildInstalledClickList();

    void installerBusyChanged();

Q_SIGNALS:
    void installerChanged();
    void repositoryListFetched();

private:
    QList<ApplicationItem*> m_list;
    QHash<QString, QString> m_installedAppIds; // appid, version

    QNetworkAccessManager *m_nam;
    ClickInstaller *m_installer;
};

#endif // APPMODEL_H
