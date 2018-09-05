#include "result_table.h"

#include <QScrollBar>
#include <QHeaderView>

CResultTable::CResultTable(QWidget* parent)
    :QTableView(parent)
{
    Init();

    /*
      //connect the headers and scrollbars of both tableviews together
      connect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this,
              SLOT(updateSectionWidth(int,int,int)));
      connect(verticalHeader(),SIGNAL(sectionResized(int,int,int)), this,
              SLOT(updateSectionHeight(int,int,int)));
    */
}

CResultTable::~CResultTable()
{
}

void CResultTable::Init()
{
      setHorizontalScrollMode(ScrollPerItem);
      setVerticalScrollMode(ScrollPerItem);
}

void CResultTable::SetModel(QAbstractItemModel * model)
{
    setModel(model);
}


