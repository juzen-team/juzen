#include "Widgets/WizardStartPage.h"

WizardStartPage::WizardStartPage(QWidget *parent) : QWizardPage(parent)
{
}

WizardStartPage::~WizardStartPage()
{
}

bool WizardStartPage::isComplete() const
{
	return false;
}
