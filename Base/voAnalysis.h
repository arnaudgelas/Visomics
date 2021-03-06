/*=========================================================================

  Program: Visomics

  Copyright (c) Kitware, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __voAnalysis_h
#define __voAnalysis_h

// Qt includes
#include <QScopedPointer>
#include <QLatin1String>
#include <QObject>
#include <QStringList>
#include <QVariant>

class QtProperty;
class QtVariantPropertyManager;
class QtVariantProperty;
class voAnalysisPrivate;
class voDataObject;
class vtkDataObject;

class voAnalysis : public QObject
{
  Q_OBJECT
public:
  voAnalysis();
  virtual ~voAnalysis();

  QString uuid()const;

  void addInputType(const QString& inputName, const QString& inputType);

  QString inputType(const QString& inputName) const;

  int numberOfInput();

  QStringList inputNames()const;

  bool hasInput(const QString& inputName) const;

  void setInput(const QString& inputName, voDataObject * dataObject);

  voDataObject * input(const QString& inputName = QLatin1String("input")) const;

  void removeAllInputs();



  void addOutputType(const QString& outputName, const QString& outputType,
                     const QString& viewType = QString(),
                     const QString& viewPrettyName = QString(),
                     const QString& rawViewType = QString(),
                     const QString& rawViewPrettyName = QString());

  QString outputType(const QString& outputName) const;

  QString viewPrettyName(const QString& outputName, const QString& viewType);
  void setViewPrettyName(const QString& outputName, const QString& viewType, const QString& viewPrettyName);

  int numberOfOutput();

  QStringList outputNames() const;

  bool hasOutput(const QString& outputName) const;

  void setOutput(const QString& outputName, voDataObject * dataObject);

  voDataObject * output(const QString& outputName) const;

  bool hasOutputWithViewType(const QString& outputName, const QString& viewType) const;

  QStringList viewTypesForOutput(const QString& outputName)const;

//  QStringList viewTypes()const;

  bool hasOutputWithRawViewType(const QString& outputName, const QString& rawViewType) const;

  QString rawViewTypeForOutput(const QString& outputName)const;

  QString rawViewPrettyName(const QString& outputName, const QString& rawViewType);
  void setRawViewPrettyName(const QString& outputName, const QString& rawViewType, const QString& rawViewPrettyName);

  void removeAllOutputs();

  bool abortExecution()const;
  void setAbortExecution(bool abortExecutionValue);

  QString outputDirectory()const;
  void setOutputDirectory(const QString& directory);

  bool writeOutputsToFilesEnabled()const;
  void setWriteOutputsToFilesEnabled(bool enabled);

  bool run();

  void initializeInputInformation();
  void initializeOutputInformation();

  void writeOutputsToFiles(const QString& directory = QLatin1String(".")) const;
  
  typedef QHash<QString, QVariant> AvoidParserBugWithGcc420; // Hack to get around a bug in Gcc 4.2.0
  void initializeParameterInformation(
    const QHash<QString, QVariant>& parameters = AvoidParserBugWithGcc420());

  void setParameterValues(const QHash<QString, QVariant>& parameters);

  QSet<QtVariantProperty*> topLevelParameterGroups()const;

  int parameterCount()const;

  void setAcceptDefaultParameterValues(bool value);
  bool acceptDefaultParameterValues()const;


  QtVariantPropertyManager * propertyManager()const;

  virtual QString parameterDescription()const;

signals:

  void outputSet(const QString& outputName, voDataObject* dataObject, voAnalysis* analysis);

protected:

  virtual bool execute();

  virtual void setInputInformation(){}
  virtual void setOutputInformation(){}
  virtual void setParameterInformation(){}

  void addParameterGroup(const QString& label, const QList<QtProperty*> parameters);

  QtVariantProperty* parameter(const QString& id)const;

  QString enumParameter(const QString& id)const;

  QtVariantProperty*  addEnumParameter(const QString& id, const QString& label,
                                             const QStringList& choices,
                                             const QString& value = QString());

  int integerParameter(const QString& id)const;

  QtVariantProperty*  addIntegerParameter(const QString& id, const QString& label,
                                          int minimum, int maximum,
                                          int value);

  QString stringParameter(const QString& id)const;

  QtVariantProperty*  addStringParameter(const QString& id, const QString& label,
                                         const QString& value);

  double doubleParameter(const QString& id)const;

  QtVariantProperty*  addDoubleParameter(const QString& id, const QString& label,
                                         double minimum, double maximum,
                                         double value);

  bool booleanParameter(const QString& id)const;

  QtVariantProperty*  addBooleanParameter(const QString& id, const QString& label, bool value);

protected:
  QScopedPointer<voAnalysisPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(voAnalysis);
  Q_DISABLE_COPY(voAnalysis);
};

#endif
