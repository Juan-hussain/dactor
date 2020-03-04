#ifndef TRANSITIONS_H
#define TRANSITIONS_H
#include<QAbstractTransition>
#include<QEvent>
struct StringEvent : public QEvent
{
    StringEvent(const QString &val)
    : QEvent(QEvent::Type(QEvent::User+1)),
      value(val) {}

    QString value;
};
class StringTransition : public QAbstractTransition
{
public:
    StringTransition(const QString &value)
        : m_value(value) {}

protected:
  //  using QAbstractTransition::eventTest;
    virtual bool eventTest(QEvent *e) const
    {
        if (e->type() != QEvent::Type(QEvent::User+1)) // StringEvent
            return false;
        StringEvent *se = static_cast<StringEvent*>(e);
        return (m_value == se->value);
    }
    virtual void onTransition(QEvent *) {}

private:
    QString m_value;
};
#endif // TRANSITIONS_H
