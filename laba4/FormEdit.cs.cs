using System;
using System.Windows.Forms;

namespace PayrollApp
{
    public partial class FormEdit : Form
    {
        private bool isEditMode;
        private Employee currentEmployee;

        public FormEdit()
        {
            InitializeComponent();
            this.Text = "Добавить сотрудника";
            this.isEditMode = false;
            this.currentEmployee = null;
            this.btnSave.Text = "Добавить";
            txtFirstName.Leave += TxtFirstName_Leave;
            txtLastName.Leave += TxtLastName_Leave;
            txtSalary.Leave += TxtSalary_Leave;
            btnSave.Click += BtnSave_Click;
            btnCancel.DialogResult = DialogResult.Cancel;
        }

        public FormEdit(Employee employee)
        {
            InitializeComponent();
            this.Text = "Редактировать сотрудника";
            this.isEditMode = true;
            this.currentEmployee = employee;
            this.btnSave.Text = "Сохранить";
            txtFirstName.Text = employee.GetFirstName();
            txtLastName.Text = employee.GetLastName();
            txtSalary.Text = employee.GetSalary().ToString();
            txtFirstName.Leave += TxtFirstName_Leave;
            txtLastName.Leave += TxtLastName_Leave;
            txtSalary.Leave += TxtSalary_Leave;
            btnSave.Click += BtnSave_Click;
            btnCancel.DialogResult = DialogResult.Cancel;
        }

        private void FormEdit_Load(object sender, EventArgs e)
        {
            
        }

        private void TxtFirstName_Leave(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(txtFirstName.Text))
            {
                errorProvider1.SetError(txtFirstName, "Имя не может быть пустым");
            }
            else if (txtFirstName.Text.Length > 50)
            {
                errorProvider1.SetError(txtFirstName, "Максимум 50 символов");
            }
            else
            {
                errorProvider1.SetError(txtFirstName, "");
            }
        }

        private void TxtLastName_Leave(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(txtLastName.Text))
            {
                errorProvider1.SetError(txtLastName, "Фамилия не может быть пустой");
            }
            else
            {
                errorProvider1.SetError(txtLastName, "");
            }
        }

        private void TxtSalary_Leave(object sender, EventArgs e)
        {
            double salary;
            if (!double.TryParse(txtSalary.Text, out salary) || salary < 0)
            {
                errorProvider1.SetError(txtSalary, "Зарплата должна быть числом >= 0");
            }
            else
            {
                errorProvider1.SetError(txtSalary, "");
            }
        }

        private void BtnSave_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(errorProvider1.GetError(txtFirstName)) ||
                !string.IsNullOrEmpty(errorProvider1.GetError(txtLastName)) ||
                !string.IsNullOrEmpty(errorProvider1.GetError(txtSalary)))
            {
                MessageBox.Show("Исправьте ошибки в полях!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            double salary;
            if (!double.TryParse(txtSalary.Text, out salary) || salary < 0)
            {
                MessageBox.Show("Зарплата должна быть числом >= 0", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (isEditMode && currentEmployee != null)
            {
                PayrollSystem.GetInstance().UpdateEmployee(
                    currentEmployee.GetId(),
                    txtFirstName.Text,
                    txtLastName.Text,
                    salary
                );
            }
            else
            {
                PayrollSystem.GetInstance().AddEmployee(txtFirstName.Text, txtLastName.Text, salary);
            }

            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}