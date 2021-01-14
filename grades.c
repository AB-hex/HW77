/*
 ============================================================================
 Name        : HW7.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#include "grades.h"
#include "linked-list.h"



typedef struct _student student;
typedef struct _course course;
typedef struct grades grades;
typedef struct node node;


//user functions
int clone_student (void *element, void **output);
void destroy_student (void *element);
int clone_course (void *element, void **output);
void destroy_course (void *element);

struct grades { //Name of the data structure
    /*array of lists*/
    struct list *students_list;
};

struct _student {
    char *name;
    int  id;
    float avg;
    struct list *courses;

};


struct _course{
	char *name;
	int grade;
};



/* User function, clone “element” to “output”. Returns 0 on success */

int clone_student(void *element, void **output){
    if(!element || !output){
        return 1;
    }

    //
    student *new_student;
    student *out_student;

    new_student = (student*)element;

    out_student = (student*)malloc(sizeof(student));

    if(!out_student){
        return 1;
    }

    out_student->id=new_student->id;
    out_student->avg=new_student->avg;

    out_student->courses = list_init(clone_course,destroy_course);

    out_student->name = (char*)malloc(
                        sizeof(char)*(strlen(new_student->name) +1));
    
    if(out_student->name==NULL){
        free(out_student);
        return 1;
     }


    out_student->name = strcpy(out_student->name , new_student->name);
    *output = out_student;
    return 0;

};

    /*user clone func fot courses list*/
int clone_course(void *element, void **output){
    if(!element || !output) {
        return 1;
    }


    course *new_course;
    course *out_course;
    new_course = (course*)element;
    out_course = (course*)malloc(sizeof(course));
    if(!out_course){
        return 1;
    }

    out_course->grade=new_course->grade;
   
    out_course->name = (char*)malloc(sizeof(char)*(strlen(new_course->name) +1));
   
    if(out_course->name==NULL){
        free(out_course);
        return 1;
    }
   
    out_course->name = strcpy(out_course->name , new_course->name);

    *output = out_course;
    return 0;
};
/*del func for student list*/
void destroy_student (void *element) {
    if(!element) {
        return;
    }
    student *Student;
    Student = (student*)element;
    free(Student->name);

    free(Student);
};
/*del func user for courses list*/
void destroy_course (void *element) {
    if(!element) {
        return;
    }

    course *Course = (course*)element;
    free(Course->name);


    free(Course);
};




/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */


struct grades* grades_init(){
	grades *Grades;
   
    Grades = (struct grades*)malloc(sizeof (struct grades));
 

    if(Grades==NULL){
        return NULL;
    }

    Grades->students_list=list_init(clone_student, destroy_student );
    if(Grades->students_list==NULL){
        free(Grades);
    }

	return Grades;
};

/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades){
        if(!grades){
            return;
        }
        //maybe syntax wrong
        /*
        node *it = list_begin(grades->students_list);
        */
        node *it = list_begin(grades->students_list);
        student *s;
        while (it){
            s = (student*)list_get(it);
            list_destroy(s->courses);
        }
        list_destroy(grades->students_list);
        free(grades);
};

/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){
    //Check Arguments

    if(!grades || !name ){
        return 1;
    }


    
    if(list_size(grades->students_list) != 0 ){   
        node *it = list_begin(grades->students_list);    
        if( !it ){
            return 1;
        }
        
        student* s;
        while( it ){
            s = (student*)list_get(it);
                if(s->id == id) {
                    return 1;
                }

            it = list_next(it);
        }
    }


    student *new_student = (student*)malloc(sizeof(student));
    
    if(!new_student){
        return 1;
    }

    char *new_name = (char*)malloc(strlen(name)+1);
    if(!new_name){
    	free(new_student);
    	return 1;
    }

    
    new_name = strcpy(new_name, name);
    new_student->name=new_name;
    new_student->id=id;
    new_student->avg= 0;
 

    return list_push_back(grades->students_list , new_student );
};




/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
    if( !grades || grade < 0 || grade > 100 ) {
        return 1;
    }
    node *it = list_begin(grades->students_list);
    student *s;
    while( it ) {
             s = (student*)list_get(it);
            if(s->id == id) {
                break;
            }

        it = list_next(it);
    }
    if(!it){
        return 1;
    }


    
    if(list_size(s->courses)!=0){

        it = list_begin(s->courses);
        course *c;
        while( it ){
                c = (course*)list_get(it);
                if(!strcmp(c->name,name)){
                     return 1;
                    }
                it = list_next(it);
            }
    }

    /**The student don't have grade in the course yet**/
    /**avg update**/

    size_t num_of_courses = list_size(s->courses);
    s->avg = ((num_of_courses)*s->avg + (float)grade )/((float)num_of_courses  + 1);

    course *new_course = (course*)malloc(sizeof(student));
    if(new_course==NULL){
    	return 1;
    }

    char* new_name= (char*)malloc(strlen(name)+1);
    if(new_name==NULL){
        free(new_course);
        return 1;}


    new_name = strcpy(new_name,name);
    new_course->name=new_name;
    new_course->grade = grade;

    return list_push_back(s->courses , new_course);
};

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out){
    if(!grades){
        return -1;
    }

    node *it = list_begin(grades->students_list);
    student *s;
    while( it ){
        s = list_get(it);
        if(s->id == id){
            break;
        }
        it = list_next(it);
    }
    if( !it ){
        return -1; /**student was not found**/
    }

    char* name_out= (char*)malloc(strlen(s->name)+1);

    if(out==NULL){
        return 0;
    }
    
    
     
    name_out = strcpy(name_out,s->name);
    *out = name_out;
    return s->avg;

};

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id){

    if(!grades) {
        return 1;
    }


    node *it = list_begin(grades->students_list);
    student *s;

    while ( it ){
        s = (student*)list_get(it);
        if(s->id == id){
            break;
        }
        it = list_next(it);
    }

    if(!it){
        return 1;
    }

    it = list_begin(s->courses);
    course *c;
    /** print the name**/
    printf("%s %d:", s->name , s->id );

    while(it) {
        c = (course*)list_get(it);
        printf(" %s %d",c->name,c->grade);
        it = list_next(it);
        if(it){
            printf(",");
        }
    }

    printf("\n");

    return 0;

};

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades){
    if(!grades){
        return 1;
    }
    node *it = list_begin(grades->students_list);
    student *s;
    while (it){
        s = (student*)list_get(it);
        grades_print_student( grades , s->id );
        it = list_next(it);
    }
    return 0;
};

