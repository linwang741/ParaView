#==========================================================================
#
#     Program: ParaView
#
#     Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
#     All rights reserved.
#
#     ParaView is a free software; you can redistribute it and/or modify it
#     under the terms of the ParaView license version 1.2.
#
#     See License_v1.2.txt for the full ParaView license.
#     A copy of this license can be obtained by contacting
#     Kitware Inc.
#     28 Corporate Drive
#     Clifton Park, NY 12065
#     USA
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#==========================================================================
set(Module_SRCS
  pqAnimationKeyFrame.cxx
  pqAnimationKeyFrame.h
  pqAnimationModel.cxx
  pqAnimationModel.h
  pqAnimationTrack.cxx
  pqAnimationTrack.h
  pqAnimationWidget.cxx
  pqAnimationWidget.h
  pqCheckableHeaderView.cxx
  pqCheckableHeaderView.h
  pqCheckBoxPixMaps.cxx
  pqCheckBoxPixMaps.cxx
  pqCheckBoxPixMaps.h
  pqCollapsedGroup.cxx
  pqCollapsedGroup.h
  pqColorButtonEventPlayer.cxx
  pqColorButtonEventPlayer.h
  pqColorButtonEventTranslator.cxx
  pqColorButtonEventTranslator.h
  pqColorChooserButton.cxx
  pqColorChooserButton.h
  pqColorDialogEventPlayer.cxx
  pqColorDialogEventPlayer.h
  pqColorDialogEventTranslator.cxx
  pqColorDialogEventTranslator.h
  pqConnect.cxx
  pqConnect.h
  pqConsoleWidget.cxx
  pqConsoleWidgetEventPlayer.cxx
  pqConsoleWidgetEventPlayer.h
  pqConsoleWidgetEventTranslator.cxx
  pqConsoleWidgetEventTranslator.h
  pqConsoleWidget.h
  pqDelimitedTextParser.cxx
  pqDelimitedTextParser.h
  pqDiscreteDoubleWidget.cxx
  pqDiscreteDoubleWidget.h
  pqDoubleLineEdit.cxx
  pqDoubleLineEdit.h
  pqDoubleLineEditEventPlayer.cxx
  pqDoubleLineEditEventPlayer.h
  pqDoubleLineEditEventTranslator.cxx
  pqDoubleLineEditEventTranslator.h
  pqDoubleRangeDialog.cxx
  pqDoubleRangeDialog.h
  pqDoubleRangeWidget.cxx
  pqDoubleRangeWidget.h
  pqDoubleSliderWidget.h
  pqDoubleSliderWidget.cxx
  pqDoubleSpinBox.cxx
  pqDoubleSpinBox.h
  pqExpandableTableView.cxx
  pqExpandableTableView.h
  pqExpanderButton.cxx
  pqExpanderButton.h
  pqFlatTreeView.cxx
  pqFlatTreeView.h
  pqHeaderView.cxx
  pqHeaderView.h
  pqHighlightablePushButton.cxx
  pqHighlightablePushButton.h
  pqHighlightableToolButton.cxx
  pqHighlightableToolButton.h
  pqLineEdit.cxx
  pqLineEditEventPlayer.cxx
  pqLineEditEventPlayer.h
  pqLineEdit.h
  pqListWidgetCheckHelper.cxx
  pqListWidgetCheckHelper.h
  pqListWidgetItemObject.cxx
  pqListWidgetItemObject.h
  pqProgressWidget.cxx
  pqProgressWidget.h
  pqQuickLaunchDialog.cxx
  pqQuickLaunchDialog.h
  pqScaleByButton.cxx
  pqScaleByButton.h
  pqSectionVisibilityContextMenu.cxx
  pqSectionVisibilityContextMenu.h
  pqSetData.cxx
  pqSetData.h
  pqSetName.cxx
  pqSetName.h
  pqSignalAdaptors.cxx
  pqSignalAdaptors.h
  pqSpinBox.cxx
  pqSpinBox.h
  pqTableView.cxx
  pqTableView.h
  pqTextEdit.cxx
  pqTextEdit.h
  pqTreeView.cxx
  pqTreeViewExpandState.cxx
  pqTreeViewExpandState.h
  pqTreeView.h
  pqTreeViewSelectionHelper.cxx
  pqTreeViewSelectionHelper.h
  pqTreeWidgetCheckHelper.cxx
  pqTreeWidgetCheckHelper.h
  pqTreeWidget.cxx
  pqTreeWidget.h
  pqTreeWidgetItem.cxx
  pqTreeWidgetItem.h
  pqTreeWidgetItemObject.cxx
  pqTreeWidgetItemObject.h
  pqTreeWidgetSelectionHelper.cxx
  pqTreeWidgetSelectionHelper.h
  pqWaitCursor.cxx
  pqWaitCursor.h
)

##########################################################################
## Add the header files that need to moc processed.
set (QtWidgets_SRCS
#  QtWidgetsPlugin.h

  pqAnimationKeyFrame.h
  pqAnimationModel.h
  pqAnimationTrack.h
  pqAnimationWidget.h
  pqCheckableHeaderView.h
  pqCheckBoxPixMaps.h
  pqCollapsedGroup.h
  pqColorButtonEventPlayer.h
  pqColorButtonEventTranslator.h
  pqColorChooserButton.h
  pqColorDialogEventPlayer.h
  pqColorDialogEventTranslator.h
  pqConsoleWidgetEventPlayer.h
  pqConsoleWidgetEventTranslator.h
  pqConsoleWidget.h
  pqDelimitedTextParser.h
  pqDiscreteDoubleWidget.h
  pqDoubleLineEdit.h
  pqDoubleLineEditEventPlayer.h
  pqDoubleLineEditEventTranslator.h
  pqDoubleRangeDialog.h
  pqDoubleRangeWidget.h
  pqDoubleSliderWidget.h
  pqDoubleSpinBox.h
  pqExpandableTableView.h
  pqExpanderButton.h
  pqFlatTreeView.h
  pqHeaderView.h
  pqHighlightablePushButton.h
  pqHighlightableToolButton.h
  pqLineEditEventPlayer.h
  pqLineEdit.h
  pqListWidgetCheckHelper.h
  pqListWidgetItemObject.h
  pqProgressWidget.h
  pqQuickLaunchDialog.h
  pqScaleByButton.h
  pqSectionVisibilityContextMenu.h
  pqSignalAdaptors.h
  pqSpinBox.h
  pqTableView.h
  pqTextEdit.h
  pqTreeViewExpandState.h
  pqTreeView.h
  pqTreeViewSelectionHelper.h
  pqTreeWidgetCheckHelper.h
  pqTreeWidget.h
  pqTreeWidgetItemObject.h
  pqTreeWidgetSelectionHelper.h
)

include(ParaViewQt)

set(qt5_public_comps Core Gui Widgets)
set(qt5_private_comps Network)
if(APPLE)
  list(APPEND qt5_private_comps PrintSupport)
endif()

if (PARAVIEW_USE_QTHELP)
  list(APPEND Module_SRCS pqHelpWindow.cxx pqHelpWindow.h)
  list(APPEND QtWidgets_SRCS pqHelpWindow.h)
  set(${vtk-module}_EXPORT_CODE
    "#ifndef PARAVIEW_USE_QTHELP\n#  define PARAVIEW_USE_QTHELP\n#endif\n")
  list(APPEND qt5_private_comps Help)
endif()

if(PARAVIEW_USE_QTWEBENGINE)
  list(APPEND qt5_public_comps WebEngineWidgets)
endif()

pv_find_package_qt(qt_public_targets REQUIRED
  QT5_COMPONENTS ${qt5_public_comps})
pv_find_package_qt(qt_private_targets REQUIRED
  QT5_COMPONENTS ${qt5_private_comps})

set(${vtk-module}_EXPORT_CODE_INSTALL
  "include(\${ParaView_CMAKE_DIR}/ParaViewQt.cmake)
  pv_find_package_qt(__${vtk-module}_qt_targets REQUIRED QUIET
    QT5_COMPONENTS ${qt5_public_comps})")
set(${vtk-module}_EXPORT_CODE_BUILD "${${vtk-module}_EXPORT_CODE_INSTALL}")

pv_qt_wrap_cpp(QtWidgets_MOC_SOURCES ${QtWidgets_SRCS})

##########################################################################
## Add *.ui files to be uic processed.
SET(QtWidgets_UI_FORMS
  Resources/UI/pqExpanderButton.ui
  Resources/UI/pqHelpWindow.ui
  Resources/UI/pqQuickLaunchDialog.ui
)

pv_qt_wrap_ui(QtWidgets_UI_BUILT_SOURCES ${QtWidgets_UI_FORMS})

##########################################################################
## Add resources.
SET(UI_RESOURCES
  Resources/QtWidgets.qrc
)

pv_qt_add_resources(RCS_SOURCES ${UI_RESOURCES})

SOURCE_GROUP("Resources" FILES
  ${UI_RESOURCES}
  ${QtWidgets_UI_FORMS}
)

SOURCE_GROUP("Generated" FILES
  ${QtWidgets_MOC_SOURCES}
  ${RCS_SOURCES}
  ${QtWidgets_UI_BUILT_SOURCES}
)

SET(QT_WIDGETS_ALL_SRCS ${Module_SRCS} ${QtWidgets_MOC_SOURCES}
  ${RCS_SOURCES} ${QtWidgets_UI_BUILT_SOURCES})

##########################################################################
# import Qt build settings
set(${vtk-module}_NO_HeaderTest 1)

vtk_module_export_code_find_package(Qt5 COMPONENTS ${qt5_public_comps})
vtk_module_library(${vtk-module} ${QT_WIDGETS_ALL_SRCS})
target_link_libraries(${vtk-module}
  LINK_PRIVATE ${qt_private_targets}
  LINK_PUBLIC ${qt_public_targets})
if(PARAVIEW_USE_QTWEBENGINE)
  target_compile_definitions(${vtk-module} PRIVATE PARAVIEW_USE_QTWEBENGINE)
endif()
