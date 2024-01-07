#include "database.h"


bool Database::connectDB(){
    

    db = QSqlDatabase::addDatabase("QMYSQL"); // Initializing Database, QMYSQL is MySQL database driver for QT.
    db.setHostName("127.0.0.1"); // Setting hostname
    db.setPort(3307); // setting port
    db.setDatabaseName("hms"); // setting database name
    db.setUserName("root"); //setting username
    db.setPassword(""); //setting password
    if(db.open()){
        return true;
    }else{ // If database connection is not established
        return false;
    }
}


QSqlDatabase Database::getDB(){
    return db;
}


bool Database::loginUser(QString email,QString password){
    QSqlQuery query;
    bool user = query.exec("SELECT * FROM users WHERE email='"+email+"' AND password='"+password+"';");


    if(user){ // condition is true if user exists
        if(query.next()){
           return true;
        }else{
           return false;
        }
    }else{
         throw "ConnectionError";
    }
//    return false;
}



void Database::addPackages(QString name, QString company, QString details, int price, bool available){
    QSqlQuery query;
    int avail = available ? 1 : 0;
    query.prepare("INSERT INTO packages(name,company,details,price,available) "
                      "VALUES (:name, :company, :details, :price, :available)");
    query.bindValue(":name", name);
    query.bindValue(":company", company);
    query.bindValue(":details", details);
    query.bindValue(":price", price);
    query.bindValue(":available",avail);
    query.exec();
}

bool Database::removePackages(int id){
    QSqlQuery query;
    query.prepare("DELETE FROM packages WHERE id=:id");
    query.bindValue(":id", id);
    if(query.exec()){
        return true;
    }else {
    return false;
}
}

Package Database::getPackageByID(int id){
    Package pkg;
    QSqlQuery query;
    query.prepare("SELECT * FROM packages WHERE id=:id");
    query.bindValue(":id", id);
    if(query.exec()){
            query.first();
            pkg.setName(query.value(1).toString());
            pkg.setCompany(query.value(2).toString());
            pkg.setDetails(query.value(3).toString());
            pkg.setPrice(query.value(4).toInt());
            pkg.setavailable(false);
    }
    return pkg;
}







Guest Database::getGuestDetailByRoomNo(int no){
    Guest guest;
    QSqlQuery query;
    query.prepare("SELECT * FROM guests WHERE room_no=:no and status='Active'");
    query.bindValue(":no", no);
    if(query.exec()){
        query.first();
        guest.setID(query.value(0).toInt());
        guest.setRoomNo(query.value(1).toInt());
        guest.setName(query.value(2).toString());
        guest.setEmail(query.value(3).toString());
        guest.setContact(query.value(4).toString());
        guest.setAddress(query.value(5).toString());
        guest.setCheckin(query.value(6).toString());
        guest.setCheckout(query.value(7).toString());
        guest.setIdentity(query.value(8).toString());
        guest.setRoomType(query.value(9).toString());
        guest.setTotalAmount(query.value(10).toInt());
        guest.setPaidAmount(query.value(11).toInt());
        guest.setDueAmount(query.value(12).toInt());
        guest.setStatus(query.value(13).toString());
        guest.setPackages(query.value(14).toString());

    }
    return guest;
}




Guest Database::getGuestDetailByID(int no){
    Guest guest;
    QSqlQuery query;
    query.prepare("SELECT * FROM guests WHERE id=:no and status='Active'");
    query.bindValue(":no", no);
    if(query.exec()){
        query.first();
        guest.setID(query.value(0).toInt());
        guest.setRoomNo(query.value(1).toInt());
        guest.setName(query.value(2).toString());
        guest.setEmail(query.value(3).toString());
        guest.setContact(query.value(4).toString());
        guest.setAddress(query.value(5).toString());
        guest.setCheckin(query.value(6).toString());
        guest.setCheckout(query.value(7).toString());
        guest.setIdentity(query.value(8).toString());
        guest.setRoomType(query.value(9).toString());
        guest.setTotalAmount(query.value(10).toInt());
        guest.setPaidAmount(query.value(11).toInt());
        guest.setDueAmount(query.value(12).toInt());
        guest.setStatus(query.value(13).toString());
        guest.setPackages(query.value(14).toString());

    }
    return guest;
}



bool Database::guestCheckOut(int id, int id2){
    QSqlQuery query;
    QSqlQuery roomQ;
//    QString checkout_date = QDate::currentDate().toString("dd.mm.yyyy");
    QDateTime checkout = QDateTime::currentDateTime();

    query.prepare("UPDATE guests SET status='InActive',checkout=:checkoutdate WHERE id=:id");
    roomQ.prepare("UPDATE room SET room_status='UNOCCUPIED' WHERE room_no=:id2");

    query.bindValue(":id",id);
    query.bindValue(":checkoutdate",checkout);
    query.exec();

    roomQ.bindValue(":id2",id2);
    if(roomQ.exec()){
        return true;
    }else {
        return false;
    }
}

// Database db;
// db.guestCheckOut(guest_id);

bool Database::isCheckOutAble(int id){
    QSqlQuery query;
    query.prepare("SELECT due_amount FROM guests where id=:id");
    query.bindValue(":id",id);
    if(query.exec()){
        query.next();
        int da = query.value(0).toInt();
        if(da == 0){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

// Database db;
// db.isCheckOutAble(guest_id);


bool Database::payAmount(int amount, int id){
    QSqlQuery query;
    int ta, pa, da;
    query.prepare("SELECT * FROM guests WHERE id=:id");
    query.bindValue(":id",id);
    if(query.exec()){
        query.next();
        ta = query.value(10).toInt();
        pa = query.value(11).toInt();
        da = query.value(12).toInt();

        int possible = pa+amount;
        if(possible > ta){
            return false;
        }else{
            int npa = pa+amount;
            int nda = da-amount;
            query.prepare("UPDATE guests SET paid_amount=:npa,due_amount=:nda WHERE id=:id");
            query.bindValue(":npa",npa);
            query.bindValue(":nda",nda);
            query.bindValue(":id",id);
            if(query.exec()){
                return true;
            }else{
                return false;
            }
        }
    }else{
        return false;
    }
}

// Database db;
// db.payAmount(amount to pay);

