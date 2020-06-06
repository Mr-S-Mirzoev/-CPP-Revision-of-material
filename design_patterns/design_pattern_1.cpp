#include <vector>
#include <iostream>
#include <string>
#include <set>
 
class Worker
{
    std::string name;
public:
    Worker (const std::string &n): name(n) {}
    bool operator< (const Worker & w) const {
        return (name < w.name);
    }
    std::string repr () const {
        return "        ----" + name + "\n";
    }
};
 
class Department
{
    std::string name;
    std::set<Worker> workers;
public:
    Department (const std::string &n): name(n) {}
    void newwrk (const std::string &wrk) {
        workers.insert(wrk);
    }
    void delwrk (const std::string &wrk) {
        auto search = workers.find(wrk);
        if (search != workers.end()) {
            workers.erase(search);
        } else {
            throw std::logic_error("There was no worker with name "+wrk);
        }
    }
    bool operator< (const Department & w) const {
        return (name < w.name);
    }
    std::string repr () const {
        std::string s;
        s += "    ====" + name + "\n";
        for (auto &wrk : workers) {
            s += wrk.repr();
        }
        return s;
    }
};
 
struct DeptPtrComp
{
    bool operator() (const Department *lhs, const Department *rhs) const {
        return ((*lhs) < (*rhs));
    }
};
 
class Company
{
    std::set<Department *, DeptPtrComp> dpts;
    std::string name;
public:
    Company (): name("Roga i kopita") {}
    void show () const {
        std::string s;
        s = ">>>>" + name + "\n";
        for (auto &dpt : dpts) {
            s += dpt->repr();
        }
        std::cout << s;
    }
    void newdp (const std::string &dp) {
        auto search = dpts.find(new Department(dp));
        if (search == dpts.end()) {
            dpts.insert(new Department(dp));
        } else {
            throw std::logic_error("There is a dept with the same name "+dp);
        }
    }
    void deldp (const std::string &dp) {
        auto search = dpts.find(new Department(dp));
        if (search != dpts.end()) {
            dpts.erase(search);
        } else {
            throw std::logic_error("There was no dept with name "+dp);
        }
    }
    void newwrk (const std::string &dpt, const std::string &wrk) {
        auto search = dpts.find(new Department(dpt));
        if (search != dpts.end()) {
            (*search)->newwrk(wrk);
        } else {
            throw std::logic_error("There was no dept with name "+dpt);
        }
    }
    void delwrk (const std::string &dpt, const std::string &wrk) {
        auto search = dpts.find(new Department(dpt));
        if (search != dpts.end()) {
            (*search)->delwrk(wrk);
        } else {
            throw std::logic_error("There was no dept with name "+dpt);
        }
    }
    void load(std::string filename) {
 
    }
    void save(std::string filename) {
 
    }
};
 
class Command
{
protected:    
    Company *comp;
public:
    virtual ~Command() {}
    virtual void Execute() = 0;
    virtual void unExecute() = 0;
 
    void setCompany (Company *c) {
        comp = c;
    }
};
 
class AddDeptCommand: public Command
{
    std::string name;
public:
    AddDeptCommand (const std::string &n): name(n) {}
    void Execute() {
        comp->newdp(name);
    }
    void unExecute() {
        comp->deldp(name);
    }
};
 
class AddWorkerCommand: public Command
{
    std::string dpt, wrk;
public:
    AddWorkerCommand (const std::string &d, const std::string &w): dpt(d), wrk(w) {}
    void Execute() {
        comp->newwrk(dpt, wrk);
    }
    void unExecute() {
        comp->delwrk(dpt, wrk);
    }
};
 
class Invoker
{
    std::vector <Command *> DoneCommands;
    std::vector <Command *> CanceledCommands;
    Company com;
    Command *command;
public:
    void addwrk (const std::string &d, const std::string &w) {
        CanceledCommands.clear();
        command = new AddWorkerCommand(d, w);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void adddpt (const std::string &d) {
        CanceledCommands.clear();
        command = new AddDeptCommand(d);
        command->setCompany(&com);
        command->Execute();
        DoneCommands.push_back(command);
    }
    void Undo() {
        if (DoneCommands.size() == 0) {
            std::cerr << "There's nothing to undo" << std::endl;
        } else {
            command = DoneCommands.back();
            DoneCommands.pop_back();
            command->unExecute();
            CanceledCommands.push_back(command);
        }
    }
    void Redo() {
        if (CanceledCommands.size() == 0) {
            std::cerr << "There's nothing to redo" << std::endl;
        } else {
            command = CanceledCommands.back();
            CanceledCommands.pop_back();
            command->Execute();
            DoneCommands.push_back(command);
        }
    }
    void Show() {
        com.show();
    }
};
 
int main()
{
    std::string com, val;
    Invoker inv;
    std::cin >> com;
    while (com != "exit") {
        if (com != "show" && com != "undo" && com != "redo") {
            std::cin >> val;
            if (com == "addwrk") {
                std::string dpt, wrk;
                int i = 0;
                while (val[i] != ':') {
                    dpt += val[i];
                    ++i;
                }
                i += 2;
                while (i < val.length()) {
                    wrk += val[i];
                    ++i;
                }
                inv.addwrk(dpt, wrk);
            } else if (com == "adddpt") {
                inv.adddpt(val);
            } else {
                std::cerr << "Unknown command" << std::endl;
            }
        } else {
            if (com == "undo") {
                inv.Undo();
            } else if (com == "redo") {
                inv.Redo();
            } else {
                inv.Show();
            }
        }
        std::cin >> com;
    }
}