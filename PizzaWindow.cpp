#include "PizzaWindow.h"

#include <QColor>
#include <QPalette>

#include "Headers/PizzaBuilder.h"

namespace {

QString TextoCantidad(const std::string& nombre, int cantidad) {
    const QString titulo = QString::fromStdString(nombre);
    if (cantidad <= 0) {
        return titulo + QStringLiteral(" — agotado");
    }
    if (cantidad == 1) {
        return titulo + QStringLiteral(" — 1 disponible");
    }
    return titulo + QStringLiteral(" — ") + QString::number(cantidad) + QStringLiteral(" disponibles");
}

}  // namespace

PizzaWindow::PizzaWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    aplicarEstilo();

    try {
        pizza_ = std::make_unique<PizzaBuilder>();
    } catch (const std::exception& ex) {
        mostrarEstado(ex.what(), false);
        ui.btnAgregarTomate->setEnabled(false);
        ui.btnAgregarQueso->setEnabled(false);
        ui.btnAgregarCarne->setEnabled(false);
        ui.btnAgregarHierba->setEnabled(false);
        ui.btnTerminar->setEnabled(false);
        return;
    }

    conectar();
    refrescar();
    mostrarEstado("Elige un ingrediente y agrégalo a la pizza.", true);
}

PizzaWindow::~PizzaWindow() = default;

void PizzaWindow::conectar()
{
    connect(ui.btnAgregarTomate, &QPushButton::clicked, this, &PizzaWindow::agregarTomate);
    connect(ui.btnAgregarQueso, &QPushButton::clicked, this, &PizzaWindow::agregarQueso);
    connect(ui.btnAgregarCarne, &QPushButton::clicked, this, &PizzaWindow::agregarCarne);
    connect(ui.btnAgregarHierba, &QPushButton::clicked, this, &PizzaWindow::agregarHierba);
    connect(ui.btnTerminar, &QPushButton::clicked, this, &PizzaWindow::terminarPizza);

    connect(ui.listTomates, &QListWidget::itemDoubleClicked, this, &PizzaWindow::agregarTomate);
    connect(ui.listQuesos, &QListWidget::itemDoubleClicked, this, &PizzaWindow::agregarQueso);
    connect(ui.listCarnes, &QListWidget::itemDoubleClicked, this, &PizzaWindow::agregarCarne);
    connect(ui.listHierbas, &QListWidget::itemDoubleClicked, this, &PizzaWindow::agregarHierba);
}

void PizzaWindow::aplicarEstilo()
{
    setStyleSheet(QStringLiteral(
        "QMainWindow, QWidget#centralWidget { background: #FFF3E4; color: #3A2418; font-family: 'Segoe UI'; font-size: 10.5pt; }"
        "QLabel#lblTitulo { font-size: 22pt; font-weight: 700; color: #8C2F1B; }"
        "QLabel#lblSubtitulo, QLabel#lblEstado { color: #6B4A38; }"
        "QGroupBox { background: #FFFBF5; border: 1px solid #E4CDB4; border-radius: 10px; margin-top: 12px; padding-top: 8px; font-weight: 700; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; padding: 0 4px; color: #8C2F1B; }"
        "QGroupBox QLabel { min-height: 18px; max-height: 22px; }"
        "QListWidget { background: #FFFFFF; color: #3A2418; border: 1px solid #E7D3C0; border-radius: 6px; outline: 0; }"
        "QListWidget::item { color: #3A2418; padding: 2px 6px; }"
        "QListWidget::item:selected { background: #F6D3C4; color: #3A2418; }"
        "QPushButton { background: #C0392B; color: white; border: none; border-radius: 6px; padding: 0 12px; min-height: 32px; max-height: 34px; font-weight: 600; }"
        "QPushButton:hover { background: #A93226; }"
        "QPushButton:disabled { background: #E4D3C6; color: #7A675C; }"
        "QPushButton#btnTerminar { background: #2F6B3A; min-height: 36px; max-height: 40px; padding: 0 18px; }"
        "QPushButton#btnTerminar:hover { background: #255832; }"
        "QPlainTextEdit { background-color: #FFFBF5; color: #3A2418; border: 1px solid #E4CDB4; border-radius: 8px; padding: 8px; selection-background-color: #F6D3C4; selection-color: #3A2418; }"
    ));

    QPalette resumen = ui.txtResumen->palette();
    resumen.setColor(QPalette::Base, QColor(QStringLiteral("#FFFBF5")));
    resumen.setColor(QPalette::Text, QColor(QStringLiteral("#3A2418")));
    resumen.setColor(QPalette::PlaceholderText, QColor(QStringLiteral("#6B4A38")));
    ui.txtResumen->setPalette(resumen);
}

void PizzaWindow::mostrarEstado(const std::string& mensaje, bool ok)
{
    ui.lblEstado->setText(QString::fromStdString(mensaje));
    ui.lblEstado->setStyleSheet(ok
        ? QStringLiteral("color: #2F6B3A; font-weight: 600;")
        : QStringLiteral("color: #8C2F1B; font-weight: 600;"));
}

std::string PizzaWindow::nombreSeleccionado(QListWidget* lista) const
{
    const QListWidgetItem* item = lista->currentItem();
    if (item == nullptr) {
        return "";
    }
    return item->data(Qt::UserRole).toString().toStdString();
}

void PizzaWindow::llenarCatalogo(QListWidget* lista, const std::vector<std::string>& nombres, const std::vector<int>& cantidades)
{
    const std::string anterior = nombreSeleccionado(lista);
    lista->clear();
    for (size_t index = 0; index < nombres.size(); ++index) {
        const int cantidad = cantidades[index];
        auto* item = new QListWidgetItem(TextoCantidad(nombres[index], cantidad), lista);
        item->setData(Qt::UserRole, QString::fromStdString(nombres[index]));
        item->setForeground(QColor(cantidad <= 0 ? QStringLiteral("#A89890") : QStringLiteral("#3A2418")));
        if (nombres[index] == anterior) {
            lista->setCurrentItem(item);
        }
    }
}

void PizzaWindow::llenarElegidos(QListWidget* lista, const std::vector<std::string>& nombres)
{
    lista->clear();
    if (nombres.empty()) {
        auto* item = new QListWidgetItem(QStringLiteral("Ninguno"), lista);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setForeground(QColor(QStringLiteral("#A89890")));
        return;
    }
    for (const auto& nombre : nombres) {
        auto* item = new QListWidgetItem(QString::fromStdString(nombre), lista);
        item->setForeground(QColor(QStringLiteral("#3A2418")));
    }
}

void PizzaWindow::refrescar()
{
    if (!pizza_) {
        return;
    }

    const auto cantidadesDe = [](const auto& repo) {
        std::vector<int> cantidades;
        cantidades.reserve(repo.Names().size());
        for (const auto& nombre : repo.Names()) {
            cantidades.push_back(repo.GetAvailableQuantity(nombre));
        }
        return cantidades;
    };

    llenarCatalogo(ui.listTomates, pizza_->Tomatoes().Names(), cantidadesDe(pizza_->Tomatoes()));
    llenarCatalogo(ui.listQuesos, pizza_->Cheeses().Names(), cantidadesDe(pizza_->Cheeses()));
    llenarCatalogo(ui.listCarnes, pizza_->Meats().Names(), cantidadesDe(pizza_->Meats()));
    llenarCatalogo(ui.listHierbas, pizza_->Herbs().Names(), cantidadesDe(pizza_->Herbs()));

    llenarElegidos(ui.elegidosTomates, pizza_->SelectedTomatoes());
    llenarElegidos(ui.elegidosQuesos, pizza_->SelectedCheeses());
    llenarElegidos(ui.elegidosCarnes, pizza_->SelectedMeats());
    llenarElegidos(ui.elegidosHierbas, pizza_->SelectedHerbs());
}

void PizzaWindow::agregarTomate()
{
    if (!pizza_ || pizza_->IsFinished()) {
        return;
    }
    const std::string nombre = nombreSeleccionado(ui.listTomates);
    if (nombre.empty()) {
        mostrarEstado("Elige un tomate de la lista.", false);
        return;
    }
    std::string mensaje;
    const bool ok = pizza_->AddTomato(nombre, mensaje);
    mostrarEstado(mensaje, ok);
    refrescar();
}

void PizzaWindow::agregarQueso()
{
    if (!pizza_ || pizza_->IsFinished()) {
        return;
    }
    const std::string nombre = nombreSeleccionado(ui.listQuesos);
    if (nombre.empty()) {
        mostrarEstado("Elige un queso de la lista.", false);
        return;
    }
    std::string mensaje;
    const bool ok = pizza_->AddCheese(nombre, mensaje);
    mostrarEstado(mensaje, ok);
    refrescar();
}

void PizzaWindow::agregarCarne()
{
    if (!pizza_ || pizza_->IsFinished()) {
        return;
    }
    const std::string nombre = nombreSeleccionado(ui.listCarnes);
    if (nombre.empty()) {
        mostrarEstado("Elige una carne de la lista.", false);
        return;
    }
    std::string mensaje;
    const bool ok = pizza_->AddMeat(nombre, mensaje);
    mostrarEstado(mensaje, ok);
    refrescar();
}

void PizzaWindow::agregarHierba()
{
    if (!pizza_ || pizza_->IsFinished()) {
        return;
    }
    const std::string nombre = nombreSeleccionado(ui.listHierbas);
    if (nombre.empty()) {
        mostrarEstado("Elige una hierba de la lista.", false);
        return;
    }
    std::string mensaje;
    const bool ok = pizza_->AddHerb(nombre, mensaje);
    mostrarEstado(mensaje, ok);
    refrescar();
}

void PizzaWindow::terminarPizza()
{
    if (!pizza_ || pizza_->IsFinished()) {
        return;
    }
    pizza_->Finish();
    ui.txtResumen->setPlainText(QString::fromStdString(pizza_->Summary()));
    ui.btnAgregarTomate->setEnabled(false);
    ui.btnAgregarQueso->setEnabled(false);
    ui.btnAgregarCarne->setEnabled(false);
    ui.btnAgregarHierba->setEnabled(false);
    ui.btnTerminar->setEnabled(false);
    mostrarEstado("Pizza lista. El inventario ya quedó guardado.", true);
}
