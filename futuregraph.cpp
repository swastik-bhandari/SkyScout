#include "futuregraph.h"
#include "ui_futuregraph.h"

futureGraph::futureGraph(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::futureGraph)
{
    ui->setupUi(this);
    setWindowTitle("Sky Scout");
    setWindowIcon(QIcon(":/new/prefix1/image/projectLogo.png"));

    ui->return_btn_3->setIcon(QIcon(":/new/prefix1/image/return.svg"));
    int y =ui->return_btn_3->height();
    ui->return_btn_3->setIconSize(QSize(y,y));

    connect(ui->return_btn_3, &QPushButton::clicked, this, &futureGraph::on_return_btn_3_clicked);
    resize(800,600);
    setFixedSize(size());

    ui->futureplot->addGraph();
    ui->futureplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->futureplot->graph(0)->setLineStyle(QCPGraph::lsLine);

    if(checkLight==false)
    {
        ui->centralWidget->setStyleSheet("background-image: url(':/new/prefix1/image/dark_bg.png');");
        ui->label->setStyleSheet("background:transparent;color:#F0EFF9;font: 10pt 'Segoe UI';");
        ui->location->setStyleSheet("background:transparent;color:#F0EFF9;font: 10pt 'Segoe UI';");
    }
    else
    {

        ui->centralWidget->setStyleSheet("background-image: url(':/new/prefix1/image/background.png');");
        ui->label->setStyleSheet("background:transparent;color:black;font: 10pt 'Segoe UI';");
        ui->location->setStyleSheet("background:transparent;color:black;font: 10pt 'Segoe UI';");

    }
    // Set axis labels
    ui->futureplot->xAxis->setLabel("Date");
    ui->futureplot->yAxis->setLabel("Temperature(°C)");

    // Customize axis range
    ui->futureplot->xAxis->setRange(24, 30); // Adjust the range to fit all labels
    ui->futureplot->yAxis->setRange(0, 100);

    // Set interactions
    ui->futureplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    QVector <double> temp;
    QVector <int> month;
    QVector <double> date;
    QVector <int> year;
    for(int i=1;i<=4;i++) {
        temp.append(futureWeather::fetchTemp(i));
        month.append(futureWeather::fetchIntMonth(i));
        date.append(futureWeather::fetchDate(i));
        year.append(futureWeather::fetchYear(i));
    }

    QVector <double> copyDate = date;

    for(int i=0;i<temp.size();i++) {
        double testDate = date[i];
        int testMonth = month[i];

        if(testMonth!=month[0]) {
            testDate = logicMaths::noOfDays(month[0],year[0]) + date[i];
        }

        date[i] = testDate;

    }

    ui->futureplot->graph(0)->setData(date, temp);

    // Customize x-axis to show date labels
    ui->futureplot->xAxis->setSubTicks(false);
    ui->futureplot->xAxis->setTickLength(0, 4);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for(int i=0;i<temp.size();i++) {
        double testDate = date[i];
//        int testMonth = month[i];

        // if(testMonth!=month[0]) {
        //     testDate = logicMaths::noOfDays(month[0],year[0]) + date[i];
        // }

        textTicker->addTick(testDate, logicMaths::nameOfMonth(month[i])+" "+QString::number(copyDate[i]));
    }

    ui->futureplot->xAxis->setTicker(textTicker);

    // Customize y-axis range if necessary
    ui->futureplot->yAxis->setRange(0, *std::max_element(temp.begin(), temp.end()) + 1);

    ui->futureplot->rescaleAxes();
    ui->futureplot->replot();
    ui->futureplot->update();

    QSqlDatabase fdb=QSqlDatabase::addDatabase("QSQLITE");
    fdb.setDatabaseName(QCoreApplication::applicationDirPath()+"/database/futuredata.db");
    fdb.open();

    QSqlQuery query;
    query.prepare("SELECT city FROM futureWeather WHERE id = 1");
    query.bindValue("1",1);
    query.exec();
    query.next();

    QString city = query.value(0).toString();

    if(check==false)
    {
        ui->label->setText("पूर्वानुमानित मौसम");
        ui->location->setText("स्थान: "+city);
    }
    else
    {
        ui->label->setText("Forecast Weather");
        ui->location->setText("Location: "+city);
    }
}

futureGraph::~futureGraph()
{
    delete ui;
}

void futureGraph::on_return_btn_3_clicked()
{
    emit returnToIntermediateWindow();
    this->close();
    return;
}

