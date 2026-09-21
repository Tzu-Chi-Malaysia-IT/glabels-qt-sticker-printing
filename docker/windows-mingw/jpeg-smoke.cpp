#include <QCoreApplication>
#include <QImageReader>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QTextStream err(stderr);

    QCoreApplication app(argc, argv);

    out << "Plugin paths:" << Qt::endl;
    for (const QString &path : QCoreApplication::libraryPaths())
        out << "  " << path << Qt::endl;

    out << "Supported formats:";
    for (const QByteArray &format : QImageReader::supportedImageFormats())
        out << " " << format;
    out << Qt::endl;

    if (argc != 2)
    {
        err << "Usage: jpeg-smoke.exe <image-file>" << Qt::endl;
        return 2;
    }

    QImageReader reader(QString::fromLocal8Bit(argv[1]));
    const QImage image = reader.read();

    if (image.isNull())
    {
        err << "JPEG load failed: " << reader.errorString() << Qt::endl;
        return 1;
    }

    out << "JPEG loaded: "
        << image.width() << "x" << image.height()
        << ", format=" << reader.format()
        << Qt::endl;

    return 0;
}
