cmake_minimum_required(VERSION 3.21)
project(geeekybash C)

set(CMAKE_C_STANDARD 23)

add_executable(geeekybash main.c execution/job.c execution/job.h execution/cd.c execution/cd.h execution/echo.c execution/echo.h execution/export.c execution/export.h execution/unset.c execution/unset.h execution/exit.c execution/exit.h execution/command.c execution/command.h store/envVar.c store/envVar.h store/jobQueue.c store/jobQueue.h utils/path.c utils/path.h utils/signal.c utils/signal.h io/reader.c io/reader.h io/parser.c io/parser.h io/output.c io/output.h utils/string.c utils/string.h store/history.c store/history.h utils/file.c utils/file.h)
