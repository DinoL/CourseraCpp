#include <iostream>
#include <iomanip> // pour setfill
#include <string>
#include <vector>
using namespace std;

typedef string Day; // type pour définir les jous

void print_time(double); // fonction utile pour afficher les heures

class Time
/* Représente le jour et l'heure d'un évènement.
 * Les heures sont représentées en double depuis minuit.
 * Par exemple 14:30 est représenté 14.5.
 * La fonction-outil print_time permet d'afficher de telles heures.
 */
{
public:
  // Constructeur à partir du jour et de l'heure
  Time(Day jour, double heure)
    : day_(jour), hour_(heure) {
  }

  // Affichage
  void print() const {
    cout << day_ << " à ";
    print_time(hour_);
  }

  // Pour connaître le jour
  Day day() const {
    return day_;
  }

  // Pour connaître l'heure
  double hour() const {
    return hour_;
  }

private:
  Day day_;
  double hour_;
};

/* Fonction outil pour afficher les temps représentés en double
 * comme indiqué ci-dessus.
 */
void print_time(double t)
{
  cout << setfill('0') << setw(2) << int(t)
       << setfill('0') << setw(1) << ":"
       << setfill('0') << setw(2) << int(60.0 * (t - int(t)));
}

/* Type utilisé pour identifier les cours.
 * Chaque cours aura en effet un identificateur unique ; par exemple "CS-101".
 */
typedef string CourseId;

/*******************************************
 * Complétez le programme à partir d'ici.
 *******************************************/
class Activity
{
public:
  Activity(string location, Day day, double hour, double duration) :
    location_(location), duration_(duration), startingTime_(day, hour)
    {}
    
  Activity(const Activity& activity) = delete;
    
  string getLocation() const { return location_; }
  Time getTime() const { return startingTime_; }
  double getDuration() const { return duration_; }
  
  bool conflicts(const Activity& activity) const
  {
      const Time& timeFirst = getTime();
      const Time& timeSecond = activity.getTime();
      if(timeFirst.day() != timeSecond.day())
      {
          return false;
      }
      
      if(timeFirst.hour() < timeSecond.hour())
      { return (timeFirst.hour() + getDuration()) > timeSecond.hour(); }
      else
      { return (timeSecond.hour() + activity.getDuration()) > timeFirst.hour(); }
  }
  void print() const
  {
      cout << "le ";
      getTime().print();
      cout << " en " << getLocation() << ", durée ";
      print_time(getDuration());
  }
  
private:
    string location_;
    double duration_;
    Time startingTime_;
};

class Course
{
public:
    Course(Course& course) = delete;
    Course(CourseId id, string name, const Activity& lecture, const Activity& exercise, int credits) :
    id_(id), name_(name), lecture_(lecture), exercise_(exercise), credits_(credits)
    {
        cout << "Nouveau cours : " << getId() << endl;
    }
    ~Course()
    {
        cout << "Suppression du cours : " << getId() << endl;
    }
    
    CourseId getId() const
    { return id_; }
    string getTitle() const
    { return name_; }
    int getCredits() const
    { return credits_; }
    
    double workload() const
    {
        return lecture_.getDuration() + exercise_.getDuration();
    }
    
    bool conflicts(const Activity& activity) const
    {
        return (lecture_.conflicts(activity) || exercise_.conflicts(activity));
    }
    
    bool conflicts(const Course& course) const
    {
        return (course.conflicts(lecture_) || course.conflicts(exercise_));
    }
    
    void print() const
    {
        cout << getId() << ": " << getTitle() << " - cours ";
        lecture_.print();
        cout << ", exercices ";
        exercise_.print();
        cout << ". crédits : " << getCredits();
    }
    
private:
  CourseId id_;
  string name_;
  const Activity& lecture_;
  const Activity& exercise_;
  int credits_;
};

class StudyPlan
{
public:
    void add_course(const Course& course)
    {
        courses.push_back(&course);
    }
    
    bool conflicts(CourseId id, vector<CourseId> selection) const
    {
        const Course* firstCourse = findCourseById(id);
        if(!firstCourse)
        { return false; }
        
        for(CourseId courseId : selection)
        {
            const Course* secondCourse = findCourseById(courseId);
            if(!secondCourse)
            { continue; }
            
            if(firstCourse->conflicts(*secondCourse))
            { return true; }
        }
        return false;
    }
    
    void print(CourseId id) const
    {
        const Course* course = findCourseById(id);
        if(course)
        { course->print(); }
    }
    
    int credits(CourseId id) const
    {
        const Course* course = findCourseById(id);
        if(course)
        {
            return course->getCredits();
        }
        else
        {
            return 0;
        }
    }
    
    double workload(CourseId id) const
    {
        const Course* course = findCourseById(id);
        if(course)
        {
            return course->workload();
        }
        else
        {
            return 0.0;
        }
    }
    
    void printCourseSuggestions(vector<CourseId> selection) const
    {
        int foundCount = 0;
        for(const Course* plannedCourse : courses)
        {
            bool printCourse = true;
            if(!plannedCourse)
            { continue; }
            
            for(CourseId courseId : selection)
            {
                const Course* course = findCourseById(courseId);
                if(!course)
                { continue; }
            
                if(plannedCourse->conflicts(*course))
                {
                    printCourse = false;
                    break;
                }
            }
            if(printCourse)
            {
                ++foundCount;
                plannedCourse->print();
                cout << endl;
            }
        }
        if(foundCount == 0)
        {
            cout << "Aucun cours n'est compatible avec la sélection de cours." << endl;
        }
    }
    
private:
    vector<const Course*> courses;
    
    const Course* findCourseById(CourseId id) const
    {
        for(const Course* course : courses)
        {
            if(course && course->getId() == id)
            {
                return course;
            }
        }
        return nullptr;
    }
};

class Schedule
{
public:
    Schedule(const StudyPlan& plan) : plan_(plan)
    {}
    
    bool add_course(CourseId id)
    {
        if(plan_.conflicts(id, selectedCourses_))
        {
            return false;
        }
        else
        {
            selectedCourses_.push_back(id);
            return true;
        }
    }
    
    double computeDailyWorkload() const
    {
        double workloadTotal = 0.0;
        for(CourseId id : selectedCourses_)
        {
            workloadTotal += plan_.workload(id);
        }
        return workloadTotal/5.0;
    }
    
    int computeTotalCredits () const
    {
        double creditsTotal = 0.0;
        for(CourseId id : selectedCourses_)
        {
            creditsTotal += plan_.credits(id);
        }
        return creditsTotal;
    }
    
    void print() const
    {
        for(CourseId id : selectedCourses_)
        {
            plan_.print(id);
            cout << endl;
        }
        cout << "Total de crédits   : " << computeTotalCredits() << endl;
        cout << "Charge journalière : ";
        print_time(computeDailyWorkload());
        cout << endl;
        cout << "Suggestions :" << endl;
        plan_.printCourseSuggestions(selectedCourses_);
        cout << endl;
    }
    
private:
    vector<CourseId> selectedCourses_;
    const StudyPlan& plan_;
};

/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Quelques activités
  Activity physicsLecture  ("Central Hall", "lundi",  9.25, 1.75);
  Activity physicsExercises("Central 101" , "lundi", 14.00, 2.00);

  Activity historyLecture  ("North Hall", "lundi", 10.25, 1.75);
  Activity historyExercises("East 201"  , "mardi",  9.00, 2.00);

  Activity financeLecture  ("South Hall",  "vendredi", 14.00, 2.00);
  Activity financeExercises("Central 105", "vendredi", 16.00, 1.00);

  // On affiche quelques informations sur ces activités
  cout << "L'activité physicsLecture a lieu ";
  physicsLecture.print();
  cout << "." << endl;

  cout << "L'activité historyLecture a lieu ";
  historyLecture.print();
  cout << "." << endl;

  if (physicsLecture.conflicts(historyLecture)) {
    cout << "physicsLecture est en conflit avec historyLecture."
         << endl;
  } else {
    cout << "physicsLecture n'est pas en conflit avec historyLecture."
         << endl;
  }
  cout << endl;

  // Création d'un plan d'étude
  StudyPlan studyPlan;
  Course physics("PHY-101", "Physique", physicsLecture, physicsExercises, 4);
  studyPlan.add_course(physics);
  Course history("HIS-101", "Histoire", historyLecture, historyExercises, 4);
  studyPlan.add_course(history);
  Course finance("ECN-214", "Finance" , financeLecture, financeExercises, 3);
  studyPlan.add_course(finance);

  // Première tentative d'emploi du temps
  Schedule schedule1(studyPlan);
  schedule1.add_course(finance.getId());
  cout << "Emploi du temps 1 :" << endl;
  schedule1.print();

  /* On ne sait pas encore très bien quoi faire : on essaye donc
   * sur une copie de l'emploi du temps précédent.
   */
  Schedule schedule2(schedule1);
  schedule2.add_course(history.getId());
  cout << "Emploi du temps 2 :" << endl;
  schedule2.print();

  // Un troisième essai
  Schedule schedule3(studyPlan);
  schedule3.add_course(physics.getId());
  cout << "Emploi du temps 3 :" << endl;
  schedule3.print();

  return 0;
}
