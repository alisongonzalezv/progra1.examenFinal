#pragma once

#include <memory>

#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>

#include "ui_PizzaWindow.h"

class PizzaBuilder;

class PizzaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PizzaWindow(QWidget* parent = nullptr);
    ~PizzaWindow() override;

private:
    void conectar();
    void aplicarEstilo();
    void refrescar();
    void agregarTomate();
    void agregarQueso();
    void agregarCarne();
    void agregarHierba();
    void terminarPizza();
    void mostrarEstado(const std::string& mensaje, bool ok);
    void llenarCatalogo(QListWidget* lista, const std::vector<std::string>& nombres, const std::vector<int>& cantidades);
    void llenarElegidos(QListWidget* lista, const std::vector<std::string>& nombres);
    std::string nombreSeleccionado(QListWidget* lista) const;

    Ui::PizzaWindowClass ui;
    std::unique_ptr<PizzaBuilder> pizza_;
};
