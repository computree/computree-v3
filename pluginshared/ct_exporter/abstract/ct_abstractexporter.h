#ifndef CT_ABSTRACTEXPORTER_H
#define CT_ABSTRACTEXPORTER_H

#include "pluginShared_global.h"

#include "interfaces.h"

class CT_AbstractExporterPrivate;
class CT_AbstractCloudIndex;

class PLUGINSHAREDSHARED_EXPORT CT_AbstractExporter : public QObject
{
    Q_OBJECT

public:
    CT_AbstractExporter();
    ~CT_AbstractExporter();

    /**
     * @brief Init your exporter (you can init for example your export formats, etc...)
     */
    virtual void init() = 0;

    /**
     * @brief Configure your exporter. Returns false if configuration is canceled.
     */
    virtual bool configureExport() { return true; }

    /**
     * @brief Return Exporter class name
     *
     *        call "metaObject()->className()"
     */
    QString getExporterName() const;

    /**
     * @brief Return Exporter custom name
     *
     *        by default return getExporterName()
     */
    virtual QString getExporterCustomName() const;

    /**
      * \brief Return available export formats (per example : CSV or XML or DXF, etc...)
      *
      *        Use the method addNewExportFormat() in the init() method to add new format.
      */
    const QList<FileFormat> &exportFormats() const;

    /**
     * @brief Inherit this method if you want to check something or filter the list of ItemDrawable
     * @warning Always call method of class parent !
     */
    virtual bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportPoints()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportFaces()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setFacesToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief Inherit this method if you want to check something. CloudIndex will be backup if the method "canExportEdges()" returns true
     * @warning Always call method of class parent !
     */
    virtual bool setEdgesToExport(const QList<CT_AbstractCloudIndex*> &list);

    /**
     * @brief Returns true if this exporter only export groups
     */
    bool exportOnlyGroup() const;

    /**
     * @brief return true if the exporter can export ItemDrawable (true by default)
     */
    bool canExportItems() const;

    /**
     * @brief return true if the exporter can export points (false by default)
     */
    bool canExportPoints() const;

    /**
     * @brief return true if the exporter can export edges (false by default)
     */
    bool canExportEdges() const;

    /**
     * @brief return true if the exporter can export faces (false by default)
     */
    bool canExportFaces() const;

    /**
     * @brief Return true if the exporter has something to export
     */
    bool hasSomethingToExport() const;

    /**
     * @brief Called by the gui to set the document manager
     */
    void setDocumentManager(const DocumentManagerInterface *dm);

    /**
     * @brief Called by the gui to set the document where the exporter was chosen
     */
    void setMyDocument(const DocumentInterface *di);

    /**
     * @brief Called by a step if a step use the exporter
     */
    void setMyStep(const CT_VirtualAbstractStep *step);

    /**
     * @brief Set the export filepath. You can overload this method if you want ot verify something in the filepath.
     *        return false if you don't accept the filepath.
     */
    virtual bool setExportFilePath(const QString &filepath);

    /**
     * @brief export filepath getter
     */
    QString exportFilePath() const;

    /**
     * @brief Overload this method if you want to return a configuration. You must first call method of the
     *        inherited class and use the SettingsNodeGroup returned to add your configuration.
     */
    virtual SettingsNodeGroup* saveExportConfiguration() const;

    /**
     * @brief Overload this method if you want to load a configuration. You must first call method of the
     *        inherited class.
     */
    virtual bool loadExportConfiguration(const SettingsNodeGroup *root);

    /**
     * @brief Called to export ItemDrawable to the file
     */
    bool exportToFile();

    /**
     * @brief Return the error message. You can set them by a call to the method setErrorMessage()
     */
    QString errorMessage() const;

    /**
     * @brief Clear the error message
     */
    void clearErrorMessage();

    /**
      * \brief Retourne la liste des ItemDrawable a exporter
      */
    const QList<CT_AbstractItemDrawable*>& itemDrawableToExport() const;

    /**
      * \brief Retourne la liste des points a exporter
      */
    const QList<CT_AbstractCloudIndex*>& pointsToExport() const;

    /**
      * \brief Retourne la liste des faces a exporter
      */
    const QList<CT_AbstractCloudIndex*>& facesToExport() const;

    /**
      * \brief Retourne la liste des edges a exporter
      */
    const QList<CT_AbstractCloudIndex*>& edgesToExport() const;

    /**
     * @brief Return the document manager (if a gui exist)
     */
    DocumentManagerInterface* documentManager() const;

    /**
     * @brief Return the document (if a gui exist) where the exporter was chosen
     */
    DocumentInterface* myDocument() const;

    /**
     * @brief return the step
     */
    CT_VirtualAbstractStep* myStep() const;

    /**
     * @brief Return a copy of the exporter.
     */
    virtual CT_AbstractExporter* copy() const = 0;

protected:

    /**
      * \brief Ajouter un nouveau format d'exportation.
      */
    void addNewExportFormat(const FileFormat &format);

    /**
     * @brief Called by derived class in "protectedExportToFile" to inform that this exporter
     *        don't used the coordinate system
     */
    void setNotNeedToUseCoordinateSystem();

    /**
      * \brief Défini la progression et emet un signal lorsque celle-ci change
      */
    void setExportProgress(int progress);

    /**
      * \brief Défini le message d'erreur
      */
    void setErrorMessage(const QString &err);

    /**
     * @brief Define if you can export ItemDrawable
     */
    void setCanExportItems(bool e);

    /**
     * @brief Define if you can export points
     */
    void setCanExportPoints(bool e);

    /**
     * @brief Define if you can export edges
     */
    void setCanExportEdges(bool e);

    /**
     * @brief Define if you can export faces
     */
    void setCanExportFaces(bool e);

    /**
     * @brief Define if you only export groups
     */
    void setExportOnlyGroup(bool e);

    /**
      * \brief A redéfinir dans votre exporter. Export dans le fichier les éléments sélectionnés lors de la configuration.
      */
    virtual bool protectedExportToFile() = 0;

protected:
    const QScopedPointer<CT_AbstractExporterPrivate>   d_ptr;

private:
    Q_DECLARE_PRIVATE(CT_AbstractExporter)

signals:

    /**
     * @brief signal to know the progress of export
     */
    void exportInProgress(int progress);
};

#endif // CT_ABSTRACTEXPORTER_H
