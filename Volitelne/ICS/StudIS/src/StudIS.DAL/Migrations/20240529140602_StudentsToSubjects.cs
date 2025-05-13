using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace StudIS.DAL.Migrations
{
    /// <inheritdoc />
    public partial class StudentsToSubjects : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "StudentEntitySubjectEntity");

            migrationBuilder.CreateTable(
                name: "StudentsToSubjects",
                columns: table => new
                {
                    Id = table.Column<Guid>(type: "TEXT", nullable: false),
                    StudentId = table.Column<Guid>(type: "TEXT", nullable: false),
                    SubjectId = table.Column<Guid>(type: "TEXT", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_StudentsToSubjects", x => x.Id);
                    table.ForeignKey(
                        name: "FK_StudentsToSubjects_Students_StudentId",
                        column: x => x.StudentId,
                        principalTable: "Students",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_StudentsToSubjects_Subjects_SubjectId",
                        column: x => x.SubjectId,
                        principalTable: "Subjects",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_StudentsToSubjects_StudentId",
                table: "StudentsToSubjects",
                column: "StudentId");

            migrationBuilder.CreateIndex(
                name: "IX_StudentsToSubjects_SubjectId",
                table: "StudentsToSubjects",
                column: "SubjectId");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "StudentsToSubjects");

            migrationBuilder.CreateTable(
                name: "StudentEntitySubjectEntity",
                columns: table => new
                {
                    StudentsId = table.Column<Guid>(type: "TEXT", nullable: false),
                    SubjectsId = table.Column<Guid>(type: "TEXT", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_StudentEntitySubjectEntity", x => new { x.StudentsId, x.SubjectsId });
                    table.ForeignKey(
                        name: "FK_StudentEntitySubjectEntity_Students_StudentsId",
                        column: x => x.StudentsId,
                        principalTable: "Students",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_StudentEntitySubjectEntity_Subjects_SubjectsId",
                        column: x => x.SubjectsId,
                        principalTable: "Subjects",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_StudentEntitySubjectEntity_SubjectsId",
                table: "StudentEntitySubjectEntity",
                column: "SubjectsId");
        }
    }
}
