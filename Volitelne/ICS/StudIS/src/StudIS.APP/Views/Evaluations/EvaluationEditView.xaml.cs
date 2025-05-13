using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StudIS.APP.ViewModels.Evaluation;
using StudIS.BL.Models;

namespace StudIS.APP.Views.Evaluations;

public partial class EvaluationEditView
{
    public EvaluationEditView(EvaluationEditViewModel viewModel) : base(viewModel)
    {
        InitializeComponent();
    }
}