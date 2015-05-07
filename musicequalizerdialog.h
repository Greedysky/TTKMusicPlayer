#ifndef MUSICEQUALIZERDIALOG_H
#define MUSICEQUALIZERDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "core/musicobject.h"
#include "core/musicmovedialogabstract.h"

namespace Ui {
class MusicEqualizerDialog;
}

class MUSIC_EXPORT MusicEqualizerDialog : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicEqualizerDialog(QWidget *parent = 0);
    ~MusicEqualizerDialog();

signals:
    void setEqEffect(const QList<int>&);
    void setEnaleEffect(bool);
    void setSpEqEffect(MusicObject::SpecialEQ);
    void resetEqEffect();

public slots:
    void verticalSlider1(int);
    void verticalSlider2(int);
    void verticalSlider3(int);
    void verticalSlider4(int);
    void verticalSlider5(int);
    void verticalSlider6(int);
    void verticalSlider7(int);
    void verticalSlider8(int);
    void verticalSlider9(int);
    void verticalSlider10(int);
    void setEqEnable();
    void resetEq();
    void eqChoiceIndexChanged(int);

    void setEchoEffect(bool echo);
    void setMixChannelEffect(bool mix);
    void setReverseEffect(bool reverse);
    void setSideCutEffect();
    void setCenterCutEffect();
    void setRateUpEffect();
    void setRateDownEffect();
    void setPitchUpEffect();
    void setPitchDownEffect();
    void setTempoUpEffect();
    void setTempoDownEffect();
    void setFadeOutEffect();
    void setFadeInEffect();

protected:
    void setControlEnable(bool);
    void readEqInformation();
    void writeEqInformation();
    void initEqualizeValue();
    void emitParameter();

    Ui::MusicEqualizerDialog *ui;
    bool m_eable;
    bool m_eqChoiceSelected;
    QList<int> m_equalizeValue;

};

#endif // MUSICEQUALIZERDIALOG_H
