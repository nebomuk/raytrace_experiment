#include "preferences.h"
#include "raytraceconfig.h"
#include "ui_preferences.h"

#include <QSettings>
#include <QDataStream>
#include <QPushButton>

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);


    loadFromSettings();

    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&Preferences::writeSettings);
    QPushButton *resetButton = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetSettingsToDefaults()));

}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::loadFromSettings()
{
    RayTraceConfig config = QSettings().value("ray_trace_config",QVariant::fromValue(RayTraceConfig())).value<RayTraceConfig>();
    ui->recursionDepth->setValue(config.recursionDepth);
    ui->minRayDifferenceForRecursion->setValue(config.minRayDifferenceForRecursion);
    ui->minRecursionRayLength->setValue(config.minRecursionRayLength);
    ui->minRayLength->setValue(config.minRayLength);
    ui->stepSize->setValue(config.stepSize);
    ui->maxRayLength->setValue(config.maxRayLength);

    ui->debugDrawPolygon->setChecked(QSettings().value("debug_draw_polygon",true).toBool());
    ui->debugDrawRays->setChecked(QSettings().value("debug_draw_rays",false).toBool());
    ui->debugDrawRecursiveStartPoints->setChecked(QSettings().value("debug_draw_recursive_start_points",false).toBool());
    ui->debugDrawGaps->setChecked(QSettings().value("debug_draw_gaps",false).toBool());


}

void Preferences::writeSettings()
{
    RayTraceConfig config = QSettings().value("ray_trace_config",QVariant::fromValue(RayTraceConfig())).value<RayTraceConfig>();
    config.recursionDepth = ui->recursionDepth->value();
    config.minRayDifferenceForRecursion = ui->minRayDifferenceForRecursion->value();
    config.minRecursionRayLength = ui->minRecursionRayLength->value();
    config.minRayLength = ui->minRayLength->value();
    config.stepSize = ui->stepSize->value();
    config.maxRayLength = ui->maxRayLength->value();
    QSettings().setValue("ray_trace_config",QVariant::fromValue(config));


    QSettings().setValue("debug_draw_polygon",ui->debugDrawPolygon->isChecked());
    QSettings().setValue("debug_draw_rays",ui->debugDrawRays->isChecked());
    QSettings().setValue("debug_draw_recursive_start_points",ui->debugDrawRecursiveStartPoints->isChecked());
    QSettings().setValue("debug_draw_gaps",ui->debugDrawGaps->isChecked());



}

void Preferences::resetSettingsToDefaults()
{
    RayTraceConfig config;
    ui->recursionDepth->setValue(config.recursionDepth);
    ui->minRayDifferenceForRecursion->setValue(config.minRayDifferenceForRecursion);
    ui->minRecursionRayLength->setValue(config.minRecursionRayLength);
    ui->minRayLength->setValue(config.minRayLength);
    ui->stepSize->setValue(config.stepSize);
    ui->maxRayLength->setValue(config.maxRayLength);
    QSettings().setValue("ray_trace_config",QVariant::fromValue(config));
}
