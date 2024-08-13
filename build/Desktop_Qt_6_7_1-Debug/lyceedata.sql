BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "klass" (
	"id"	TEXT NOT NULL,
	"schoolYear"	TEXT NOT NULL,
	"klassName"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "student" (
	"id"	INTEGER,
	"name"	TEXT NOT NULL,
	"matricule"	TEXT UNIQUE,
	"dateNaiss"	TEXT,
	"situation"	TEXT NOT NULL,
	"sexe"	TEXT NOT NULL,
	"f_klass"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
COMMIT;
