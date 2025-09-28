#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedRow(-1)
    , darkTheme(false)
{
    ui->setupUi(this);

    // Assign widgets directly from ui
    nameEdit = ui->nameEdit;
    platformCombo = ui->platformCombo;
    subscribersSpin = ui->subscribersSpin;
    contentTypeEdit = ui->contentTypeEdit;
    searchEdit = ui->searchEdit;
    addButton = ui->addButton;
    updateButton = ui->updateButton;
    deleteButton = ui->deleteButton;
    themeButton = ui->themeButton;
    exportButton = ui->exportButton;
    clearButton = ui->clearButton;
    creatorTable = ui->creatorTable;
    stackedWidget = ui->stackedWidget;
    financeButton = ui->financeButton;
    contentCreatorButton = ui->contentCreatorButton;
    sponsorButton = ui->sponsorButton;
    dealButton = ui->dealButton;
    employeeButton = ui->employeeButton;

    // Setup table
    if (!creatorTable) {
        QMessageBox::critical(this, "Error", "Table widget not found!");
        return;
    }
    creatorTable->setColumnCount(4);
    creatorTable->setHorizontalHeaderLabels({"Name", "Platform", "Subscribers", "Content Type"});
    creatorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    creatorTable->setSelectionMode(QAbstractItemView::SingleSelection);
    creatorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(creatorTable, &QTableWidget::cellClicked, this, &MainWindow::handleTableRowSelected);

    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::handleAddButtonClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::handleUpdateButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::handleSearchTextChanged);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::handleExportButtonClicked);
    connect(themeButton, &QPushButton::clicked, this, &MainWindow::handleThemeButtonClicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::handleClearButtonClicked);

    // Connect sidebar navigation
    connect(managementButton, &QPushButton::clicked, this, &MainWindow::showManagementPage);
    connect(financeButton, &QPushButton::clicked, this, &MainWindow::showFinancePage);
    connect(contentCreatorButton, &QPushButton::clicked, this, &MainWindow::showContentCreatorPage);
    connect(sponsorButton, &QPushButton::clicked, this, &MainWindow::showSponsorPage);
    connect(dealButton, &QPushButton::clicked, this, &MainWindow::showDealPage);
    connect(employeeButton, &QPushButton::clicked, this, &MainWindow::showEmployeePage);

    loadTheme();
    updateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleAddButtonClicked()
{
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty!");
        return;
    }
    if (!isNameUnique(name)) {
        QMessageBox::warning(this, "Error", "Name must be unique!");
        return;
    }

    Creator creator;
    creator.name = name;
    creator.platform = platformCombo->currentText();
    creator.subscribers = subscribersSpin->value();
    creator.contentType = contentTypeEdit->text().trimmed();
    creators.append(creator);
    updateTable();
    handleClearButtonClicked();
}

void MainWindow::handleUpdateButtonClicked()
{
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Error", "Select a row to update!");
        return;
    }
    QString oldName = creators[selectedRow].name;
    QString newName = nameEdit->text().trimmed();
    if (newName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty!");
        return;
    }
    if (!isNameUnique(newName, oldName)) {
        QMessageBox::warning(this, "Error", "Name must be unique!");
        return;
    }

    creators[selectedRow].name = newName;
    creators[selectedRow].platform = platformCombo->currentText();
    creators[selectedRow].subscribers = subscribersSpin->value();
    creators[selectedRow].contentType = contentTypeEdit->text().trimmed();
    updateTable();
    selectedRow = -1;
    handleClearButtonClicked();
}

void MainWindow::handleDeleteButtonClicked()
{
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Error", "Select a row to delete!");
        return;
    }
    creators.removeAt(selectedRow);
    updateTable();
    selectedRow = -1;
    handleClearButtonClicked();
}

void MainWindow::handleTableRowSelected(int row, int column)
{
    Q_UNUSED(column);
    if (row >= 0 && row < creators.size()) {
        selectedRow = row;
        nameEdit->setText(creators[row].name);
        platformCombo->setCurrentText(creators[row].platform);
        subscribersSpin->setValue(creators[row].subscribers);
        contentTypeEdit->setText(creators[row].contentType);
    }
}

void MainWindow::handleSearchTextChanged(const QString &text)
{
    updateTable(text);
}

void MainWindow::handleExportButtonClicked()
{
    QString fileName = "creators.csv";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Name,Platform,Subscribers,Content Type\n";
        for (const Creator &creator : creators) {
            out << creator.name << "," << creator.platform << "," << creator.subscribers << "," << creator.contentType << "\n";
        }
        file.close();
        QMessageBox::information(this, "Success", "Exported to " + fileName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to export to " + fileName);
    }
}

void MainWindow::handleThemeButtonClicked()
{
    darkTheme = !darkTheme;
    saveTheme();
    loadTheme();
}

void MainWindow::handleClearButtonClicked()
{
    nameEdit->clear();
    platformCombo->setCurrentIndex(0);
    subscribersSpin->setValue(100); // Default to 100 as in original
    contentTypeEdit->clear();
    selectedRow = -1;
}

void MainWindow::showManagementPage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->managementPage);
    }
}

void MainWindow::showFinancePage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->financePage);
    }
}

void MainWindow::showContentCreatorPage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->contentCreatorPage);
    }
}

void MainWindow::showSponsorPage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->sponsorPage);
    }
}

void MainWindow::showDealPage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->dealPage);
    }
}

void MainWindow::showEmployeePage()
{
    if (stackedWidget) {
        stackedWidget->setCurrentWidget(ui->employeePage);
    }
}

void MainWindow::updateTable(const QString &searchText)
{
    creatorTable->setRowCount(0);
    for (int i = 0; i < creators.size(); ++i) {
        const Creator &creator = creators[i];
        if (searchText.isEmpty() || creator.name.contains(searchText, Qt::CaseInsensitive) ||
            creator.platform.contains(searchText, Qt::CaseInsensitive)) {
            int row = creatorTable->rowCount();
            creatorTable->insertRow(row);
            creatorTable->setItem(row, 0, new QTableWidgetItem(creator.name));
            creatorTable->setItem(row, 1, new QTableWidgetItem(creator.platform));
            creatorTable->setItem(row, 2, new QTableWidgetItem(QString::number(creator.subscribers)));
            creatorTable->setItem(row, 3, new QTableWidgetItem(creator.contentType));
        }
    }
}

bool MainWindow::isNameUnique(const QString &name, const QString &oldName)
{
    for (const Creator &creator : creators) {
        if (creator.name == name && name != oldName) {
            return false;
        }
    }
    return true;
}

void MainWindow::loadTheme()
{
    setProperty("darkTheme", darkTheme);
    style()->unpolish(this);
    style()->polish(this);
    update();
    if (themeButton) {
        themeButton->setText(darkTheme ? "Light Theme" : "Dark Theme");
    }
}

void MainWindow::saveTheme()
{
    QSettings settings("MyCompany", "ContentCreatorManager");
    settings.setValue("darkTheme", darkTheme);
}
