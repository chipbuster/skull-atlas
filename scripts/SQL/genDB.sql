/* This file is meant to be used to generate a standard schema for the
 * skull atlas project.
 * Let ${DB} be the file that sqlite3 uses to store the database. To use
 * this script, simply run sqlite3 ${DB} < genDB.sql.
 * Note that this will fail if the tables already exist within the db. */

create table metrics(patientid TEXT, metricname TEXT, metricval REAL);
create table patient(patientid TEXT, gender TEXT, age INTEGER);
