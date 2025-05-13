using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace StudIS.DAL.Migrations
{
    /// <inheritdoc />
    public partial class StudentAndEvaluationUpdate : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.RenameColumn(
                name: "Name",
                table: "Students",
                newName: "LastName");

            migrationBuilder.AddColumn<string>(
                name: "FirstName",
                table: "Students",
                type: "TEXT",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<int>(
                name: "Grade",
                table: "Evaluations",
                type: "INTEGER",
                nullable: false,
                defaultValue: 0);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "FirstName",
                table: "Students");

            migrationBuilder.DropColumn(
                name: "Grade",
                table: "Evaluations");

            migrationBuilder.RenameColumn(
                name: "LastName",
                table: "Students",
                newName: "Name");
        }
    }
}
