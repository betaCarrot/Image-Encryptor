#ifndef BASEFILTER_H
#define BASEFILTER_H

#include <QImage>
#include <QObject>

/*!
 * \brief An abstract base class for all filters.
 */
class BaseFilter: public QObject {
    Q_OBJECT
public:
    BaseFilter() = default;
    void setImage(QImage);
    QImage getImage();
    virtual void apply() = 0; /*!< A virtual function that all non-virtual derived filters override. */
    signals:
    void progressUpdated(int value); /*!< A signal for communicating the progress of the filtering process with MainWindow. */
public slots:
    void isCancelled();
protected:
    QImage image; /*!< An image stored inside the filter */
    bool cancelled /*!< A boolean variable used to tell if the user has pressed the "Cancel" button during a filtering process */;
};

#endif // BASEFILTER_H
