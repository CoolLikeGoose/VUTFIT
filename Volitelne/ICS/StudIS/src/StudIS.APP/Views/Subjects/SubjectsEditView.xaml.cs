using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StudIS.APP.ViewModels.Subjects;

namespace StudIS.APP.Views.Subjects;

public partial class SubjectsEditView
{
    public SubjectsEditView(SubjectsEditViewModel viewModel) : base(viewModel)
    {
        InitializeComponent();
    }
}