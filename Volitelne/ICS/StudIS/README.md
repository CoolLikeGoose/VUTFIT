# ICS project 2024

## Table of Contents

- [Executive Summary](#executive-summary)
- [Introduction](#introduction)
- [Teamwork](#teamwork)
- [Project Architecture and Design](#project-architecture-and-design)
- [Tools Used](#tools-used)
- [Bibliography](#bibliography)

## Executive Summary
The aim of the **"School information system"** project was to make a fully working, easily scalable and maintainable system for a notional university\school, which would have all the necessary functions, such as adding subjects, students, dealing with different events such as tests or exams and their evaluation.   
This system should have a visual interface and include work with the database of students and subjects.
## Introduction
The aim of the **"School information system"** project was to make a fully working, easily scalable and maintainable system for a notional university\school.
## The goal set by the teachers
The goal is to create a usable and easily extensible application that meets the requirements of the brief. The application must not crash or freeze. If the user fills in something incorrectly, they are notified with a validation message.

The assignment leaves room for custom implementation. The evaluation focuses on the technical processing and the quality of the code, however, user-friendliness and graphic design are also evaluated.
## Teamwork 
One of the challenges we faced was teamwork. In addition to creating a working and maintainable application, we also had to figure out how to do it efficiently, on time and using all available human resources. Therefore, the best way to do this was to divide responsibilities.   
##### All team members:
- Nikita Smirnov (xsmirn02)  [[Developer]](https://github.com/CoolLikeGoose)
- Daniil Zverev (xzvere00) [[Team Leader]](https://github.com/Danooone)
- Shaposhnik Bogdan (xshapo04) [[Developer]](https://github.com/Amega-h)
- Vladyslav Yeroma (xyerom00) [[Developer]](https://github.com/vVladyslaVv)
## Project Architecture and Design
### Project Architecture
To be able to further maintain and scale the application, we developed it with a high level of abstraction. The application operation is divided into several modules.
- Data Access Layer (DAL)
- Business Logic (BL)
- Common
### Data Access Layer
Responsible for data processing in the application
- `Entities:` Сontains all key entities and their properties
- `Mappers:` Mappers interface
- `UnitOfWork:` Ensures the atomicity of operations
### Business Logic
Implements facades, models, mappers and interfaces for more convenient data handling
- `Facades:` Contains facades and interfaces
- `Mappers:` Contains mappers that map entities in models and vice versa
- `Models:` Describes models
### Common
Contains some helping enums
### Data Handled
#### Student
- Name
- Last name
- Photo
- Subjects
#### Activity (exercise, exam)
- Start (date, time)
- End (date, time)
- Room
- Activity type/tag
- Activity description
- Subject
- Evaluation
#### Subject
- Name
- Abbreviation
- Activity
- Students
#### Evaluation
- Note
- Activity
- Student
## Tools Used
The client was developed on different laptops, with Windows operating systems installed. 
### Key tools used

**.NET 8+ SDK:** Provides the runtime and libraries for building the application, along with tools for compilation and package management.

**JetBrains Rider:** Served as the primary IDE for most of team members.

**Azure DevOps:** Pipelines installation,version control and repository hosting.

## Bibliography
- 18 | ICS project task | GitHub repository by nesfit | [online]. 2024 Link: (https://github.com/nesfit/ICS/tree/master/Project)
