#ifndef RESULTTABLE_H
#define RESULTTABLE_H

#include <QTableView>

class CResultTable : public QTableView {
     Q_OBJECT

public:
      CResultTable(QWidget* parent = 0);
      ~CResultTable();

      void SetModel(QAbstractItemModel * model);

protected:
      void Init();


};

#endif // RESULTTABLE_H
