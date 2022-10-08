#include "pch.h"
#include "StructurePage.h"
#include "SymbolToTreeView.h"
#include "TreeListView.h"

CStructPage::CStructPage(HANDLE hObject) : m_hObject(hObject), m_Object(DiaSymbol::Empty) {
}

void CStructPage::SetSymbol(DiaSymbol sym, PVOID address) {
    m_Object = std::move(sym);
    m_Address = address;
}

LRESULT CStructPage::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&) {
    InitDynamicLayout(false);

    m_Tree.SubclassWindow(GetDlgItem(IDC_TREE));
    m_Tree.AddColumn(L"Member", 200);
    m_Tree.AddColumn(L"Type", 180);
    m_Tree.AddColumn(L"Value", 150);
    m_Tree.AddColumn(L"Details", 150);
    m_font.CreatePointFont(100, L"Consolas");
    m_Tree.GetTreeControl().SetFont(m_font);

    CImageList images;
    images.Create(16, 16, ILC_COLOR32, 8, 0);
    UINT icons[] = { IDI_STRUCT, IDI_UNION, IDI_FIELD, IDI_ENUM, IDI_BITFIELD };
    for (auto icon : icons)
        images.AddIcon(AtlLoadIconImage(icon, 0, 16, 16));
    m_Tree.GetTreeControl().SetImageList(images);

    auto hRoot = m_Tree.GetTreeControl().InsertItem(m_Object.Name().c_str(), 0, 0, TVI_ROOT, TVI_LAST);
    SymbolToTreeView::FillTreeView(m_Tree, hRoot, m_Object);
    m_Tree.GetTreeControl().Expand(hRoot, TVE_EXPAND);

    return 0;
}


