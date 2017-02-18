cmake_minimum_required(VERSION 3.3)
project(juzen)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if (flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if (flag_supported)
            message(status "supported")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

if (NOT MSVC)
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-pedantic")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Jellow REQUIRED)
if (NOT JELLOW_FOUND)
    message(FATAL_ERROR "Jellow not found!")
endif()

include_directories("src/" ${JELLOW_INCLUDE_DIRS})

set(SOURCE_FILES
    src/main.cpp
    src/Roster/Roster.cpp
    src/Roster/Roster.h
    src/Roster/Contact.cpp
    src/Roster/Contact.h
    src/Roster/ContactResource.cpp
    src/Roster/ContactResource.h
    src/System/AppInfo.cpp
    src/System/AppInfo.h
    src/System/Settings.cpp
    src/System/Settings.h
    src/Account.cpp
    src/Account.h
    src/AccountManager.cpp
    src/AccountManager.h
    src/MainWindow.cpp
    src/MainWindow.h
    src/Widgets/ContactListView.cpp
    src/Widgets/ContactListView.h
    src/Widgets/ContactListDelegate.cpp
    src/Widgets/ContactListDelegate.h
    src/Widgets/ContactListModel.cpp
    src/Widgets/ContactListModel.h
    src/Widgets/AccountAddWizard.cpp
    src/Widgets/AccountAddWizard.h
    src/Widgets/WizardStartPage.cpp
    src/Widgets/WizardStartPage.h
    src/Widgets/AccountAddExistingWizardPage.cpp
    src/Widgets/AccountAddExistingWizardPage.h
    src/Widgets/AccountRegisterWizardPage.cpp
    src/Widgets/AccountRegisterWizardPage.h
    src/Widgets/DataFormWidget.cpp
    src/Widgets/DataFormWidget.h
    src/Widgets/DataFormFieldWidget.cpp
    src/Widgets/DataFormFieldWidget.h
    src/Widgets/AccountRegisterFormWizardPage.cpp
    src/Widgets/AccountRegisterFormWizardPage.h)

add_executable(juzen ${SOURCE_FILES})
set_target_properties(juzen PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
target_link_libraries(juzen Qt5::Core Qt5::Gui Qt5::Widgets ${JELLOW_LIBRARIES})
