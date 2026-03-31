using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace PayrollApp
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
            LoadDataToGrid();
            SetupDataGridView();
        }

        private void SetupDataGridView()
        {
            dataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dataGridView1.MultiSelect = false;
        }

        private void LoadDataToGrid()
        {
            dataGridView1.Rows.Clear();
            var employees = PayrollSystem.GetInstance().GetEmployees();

            foreach (var emp in employees)
            {
                dataGridView1.Rows.Add(
                    emp.GetId(),
                    emp.GetFirstName(),
                    emp.GetLastName(),
                    emp.CalculateSalary().ToString("F2")
                );
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            FormEdit editForm = new FormEdit();
            if (editForm.ShowDialog() == DialogResult.OK)
            {
                LoadDataToGrid();
            }
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
            {
                MessageBox.Show("Выберите сотрудника для редактирования", "Внимание",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            int selectedId = (int)dataGridView1.SelectedRows[0].Cells["colID"].Value;
            Employee emp = PayrollSystem.GetInstance().FindEmployee(selectedId);

            if (emp != null)
            {
                FormEdit editForm = new FormEdit(emp);
                if (editForm.ShowDialog() == DialogResult.OK)
                {
                    LoadDataToGrid();
                }
            }
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
            {
                MessageBox.Show("Выберите сотрудника для удаления", "Внимание",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            DialogResult result = MessageBox.Show("Удалить выбранного сотрудника?",
                "Подтверждение", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (result == DialogResult.Yes)
            {
                int selectedId = (int)dataGridView1.SelectedRows[0].Cells["colID"].Value;
                PayrollSystem.GetInstance().RemoveEmployee(selectedId);
                LoadDataToGrid();
            }
        }

        private void btnImport_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "CSV Files|*.csv|All Files|*.*";
            ofd.Title = "Импорт данных";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    PayrollSystem.GetInstance().LoadFromFile(ofd.FileName);
                    LoadDataToGrid();
                    MessageBox.Show("Данные успешно загружены!", "Успех",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Ошибка импорта: " + ex.Message, "Ошибка",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "CSV Files|*.csv";
            sfd.Title = "Экспорт данных";

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    PayrollSystem.GetInstance().SaveToFile(sfd.FileName);
                    MessageBox.Show("Данные успешно сохранены!", "Успех",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Ошибка экспорта: " + ex.Message, "Ошибка",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnSort_Click(object sender, EventArgs e)
        {
            var employees = PayrollSystem.GetInstance().GetEmployees();
            var sorted = employees.OrderByDescending(emp => emp.CalculateSalary()).ToList();

            dataGridView1.Rows.Clear();
            foreach (var emp in sorted)
            {
                dataGridView1.Rows.Add(
                    emp.GetId(),
                    emp.GetFirstName(),
                    emp.GetLastName(),
                    emp.CalculateSalary().ToString("F2")
                );
            }
        }

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            var employees = PayrollSystem.GetInstance().GetEmployees();

            if (e.ColumnIndex == 1)
            {
                var sorted = employees.OrderBy(emp => emp.GetFirstName()).ToList();
                dataGridView1.Rows.Clear();
                foreach (var emp in sorted)
                {
                    dataGridView1.Rows.Add(
                        emp.GetId(),
                        emp.GetFirstName(),
                        emp.GetLastName(),
                        emp.CalculateSalary().ToString("F2")
                    );
                }
            }
            else if (e.ColumnIndex == 3)
            {
                var sorted = employees.OrderByDescending(emp => emp.CalculateSalary()).ToList();
                dataGridView1.Rows.Clear();
                foreach (var emp in sorted)
                {
                    dataGridView1.Rows.Add(
                        emp.GetId(),
                        emp.GetFirstName(),
                        emp.GetLastName(),
                        emp.CalculateSalary().ToString("F2")
                    );
                }
            }
        }
    }
}