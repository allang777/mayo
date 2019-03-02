/****************************************************************************
** Copyright (c) 2018, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "io_handler.h"

#include <QtCore/QObject>
#include <memory>
#include <string>
#include <vector>
class QFileInfo;

namespace qttask { class Progress; }

namespace Mayo {

class Document;
class DocumentItem;
class Property;

class Application : public QObject {
    Q_OBJECT
public:
    // -- Types
    enum class PartFormat {
        Unknown,
        Iges,
        Step,
        OccBrep,
        Stl
    };

    struct ExportOptions {
#ifdef HAVE_GMIO
        gmio_stl_format stlFormat = GMIO_STL_FORMAT_UNKNOWN;
        std::string stlaSolidName;
        gmio_float_text_format stlaFloat32Format =
                GMIO_FLOAT_TEXT_FORMAT_SHORTEST_LOWERCASE;
        uint8_t stlaFloat32Precision = 9;
#else
        enum class StlFormat {
            Ascii,
            Binary
        };
        StlFormat stlFormat = StlFormat::Binary;
#endif
    };

    using ArrayDocument = std::vector<Document*>;
    using ArrayDocumentConstIterator = ArrayDocument::const_iterator;

    // -- API
    static Application* instance();

    const ArrayDocument& documents() const;
    int documentCount() const;
    Document* documentAt(int index) const;

    void addDocument(Document* doc);
    bool eraseDocument(Document* doc);

    ArrayDocumentConstIterator findDocumentByLocation(const QFileInfo& loc) const;

    static Span<const PartFormat> partFormats();
    static QString partFormatFilter(PartFormat format);
    static QStringList partFormatFilters();
    static PartFormat findPartFormat(const QString& filepath);

    Io::Result importInDocument(
            Document* doc,
            PartFormat format,
            const QString& filepath,
            qttask::Progress* progress = nullptr);
    Io::Result exportDocumentItems(
            Span<const ApplicationItem> spanAppItem,
            PartFormat format,
            const QString& filepath,
            qttask::Progress* progress = nullptr);
    static PropertyOwner* optionsImport(PartFormat format);
    static PropertyOwner* optionsExport(PartFormat format);

signals:
    void documentAdded(Document* doc);
    void documentErased(const Document* doc);
    void documentPropertyChanged(Document* doc, Property* prop);
    void documentItemAdded(DocumentItem* docItem);
    void documentItemErased(const DocumentItem* docItem);
    void documentItemPropertyChanged(DocumentItem* docItem, Property* prop);

    // -- Implementation
private:
    Application(QObject* parent = nullptr);

    std::vector<Document*> m_documents;
};

} // namespace Mayo
