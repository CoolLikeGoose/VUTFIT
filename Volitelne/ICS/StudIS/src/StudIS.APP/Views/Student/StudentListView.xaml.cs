using StudIS.APP.ViewModels.Student;

namespace StudIS.APP.Views.Student;

public partial class StudentListView
{
    public StudentListView(StudentListViewModel listViewModel) : base(listViewModel)
    {
        InitializeComponent();
    }
}