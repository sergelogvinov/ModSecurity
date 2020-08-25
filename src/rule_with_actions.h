/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#ifdef __cplusplus
#include <stack>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <utility>
#endif

#ifndef SRC_RULE_WITH_ACTIONS_H_
#define SRC_RULE_WITH_ACTIONS_H_

#include "modsecurity/transaction.h"
#include "modsecurity/modsecurity.h"
#include "modsecurity/variable_value.h"
#include "modsecurity/rule.h"
#include "modsecurity/actions/action.h"
#include "src/actions/action_type_rule_metadata.h"
#include "src/actions/action_with_execution.h"
#include "src/actions/disruptive/disruptive_action.h"


#ifdef __cplusplus

namespace modsecurity {

namespace actions {
class Action;
class Severity;
class LogData;
class Msg;
class Rev;
class SetVar;
class Tag;
class XmlNS;
namespace transformations {
class Transformation;
}
}

using Transformation = actions::transformations::Transformation;
using Transformations = std::vector<std::shared_ptr<Transformation> >;
using TransformationsPtr = std::vector<Transformation *>;

using Actions = std::vector<actions::Action *>;
using ActionWithExecution = actions::ActionWithExecution;
using ActionTypeRuleMetaData = actions::ActionTypeRuleMetaData;
using ActionDisruptive = actions::disruptive::ActionDisruptive;

using MatchActions = std::vector<std::shared_ptr<ActionWithExecution > >;
using MatchActionsPtr = std::vector<ActionWithExecution *>;

using Tags = std::vector<std::shared_ptr<actions::Tag> >;
using TagsPtr = std::vector<actions::Tag *>;

using SetVars = std::vector<std::shared_ptr<actions::SetVar> >;
using SetVarsPtr = std::vector<actions::SetVar *>;

using XmlNSs = std::vector<std::shared_ptr<actions::XmlNS> >;
using XmlNSsPtr = std::vector<actions::XmlNS *>;


class TransformationResult {
 public:
    explicit TransformationResult(
        ModSecString &after,
        const std::string *transformation = nullptr)
        : m_after(after),
        m_transformation(transformation) { };

    explicit TransformationResult(
        ModSecString *after)
        : m_after(*after),
        m_transformation(nullptr) { };

    TransformationResult(const TransformationResult &t2)
        : m_after(t2.m_after),
        m_transformation(t2.m_transformation) { };


    ModSecString *getAfter() {
        return &m_after;
    }


    const std::string *getTransformationName() const {
        return m_transformation;
    }


 private:
    ModSecString m_after;
    const std::string *m_transformation;
};

using TransformationsResults = std::list<TransformationResult>;


class RuleWithActions : public Rule {
 public:
    int SEVERITY_NOT_SET = 10;
    int ACCURACY_NOT_SET = 10;
    int MATURITY_NOT_SET = 10;

    RuleWithActions(
        Actions *a,
        Transformations *t,
        std::unique_ptr<std::string> fileName,
        int lineNumber);
    ~RuleWithActions();

    RuleWithActions(const RuleWithActions &r)
        : Rule(r),
        m_ruleId(r.m_ruleId),
        m_chainedRuleChild(r.m_chainedRuleChild),
        m_chainedRuleParent(r.m_chainedRuleParent),
        m_disruptiveAction(r.m_disruptiveAction),
        m_logData(r.m_logData),
        m_msg(r.m_msg),
        m_actionsRuntimePos(r.m_actionsRuntimePos),
        m_actionsSetVar(r.m_actionsSetVar),
        m_actionsTag(r.m_actionsTag),
        m_XmlNSs(r.m_XmlNSs),
        m_defaultActionDisruptiveAction(r.m_defaultActionDisruptiveAction),
        m_defaultActionLogData(r.m_defaultActionLogData),
        m_defaultActionMsg(r.m_defaultActionMsg),
        m_defaultActionActionsRuntimePos(r.m_defaultActionActionsRuntimePos),
        m_defaultActionActionsSetVar(r.m_defaultActionActionsSetVar),
        m_defaultActionActionsTag(r.m_defaultActionActionsTag),
        m_transformations(r.m_transformations),
        m_defaultTransformations(r.m_defaultTransformations),
        m_severity(r.m_severity),
        m_revision(r.m_revision),
        m_version(r.m_version),
        m_accuracy(r.m_accuracy),
        m_maturity(r.m_maturity),
        m_containsCaptureAction(r.m_containsCaptureAction),
        m_containsLogAction(r.m_containsLogAction),
        m_containsNoLogAction(r.m_containsNoLogAction),
        m_containsAuditLogAction(r.m_containsAuditLogAction),
        m_containsNoAuditLogAction(r.m_containsNoAuditLogAction),
        m_containsMultiMatchAction(r.m_containsMultiMatchAction),
        m_containsStaticBlockAction(r.m_containsStaticBlockAction),
        m_defaultSeverity(r.m_defaultSeverity),
        m_defaultRevision(r.m_defaultRevision),
        m_defaultVersion(r.m_defaultVersion),
        m_defaultAccuracy(r.m_defaultAccuracy),
        m_defaultMaturity(r.m_defaultMaturity),
        m_defaultContainsCaptureAction(r.m_defaultContainsCaptureAction),
        m_defaultContainsLogAction(r.m_defaultContainsLogAction),
        m_defaultContainsNoLogAction(r.m_defaultContainsNoLogAction),
        m_defaultContainsAuditLogAction(r.m_defaultContainsAuditLogAction),
        m_defaultContainsNoAuditLogAction(r.m_defaultContainsNoAuditLogAction),
        m_defaultContainsMultiMatchAction(r.m_defaultContainsMultiMatchAction),
        m_defaultContainsStaticBlockAction(r.m_defaultContainsStaticBlockAction),
        m_isChained(r.m_isChained) {
            // TODO: Verify if it is necessary to process any other copy.
        };

    RuleWithActions &operator=(const RuleWithActions& r) {
        Rule::operator = (r);
        m_ruleId = r.m_ruleId;
        m_chainedRuleChild = r.m_chainedRuleChild;
        m_chainedRuleParent = r.m_chainedRuleParent;
        m_disruptiveAction = r.m_disruptiveAction;
        m_logData = r.m_logData;
        m_msg = r.m_msg;
        m_actionsRuntimePos = r.m_actionsRuntimePos;
        m_actionsSetVar = r.m_actionsSetVar;
        m_actionsTag = r.m_actionsTag;
        m_XmlNSs = r.m_XmlNSs;
        m_defaultActionDisruptiveAction = r.m_defaultActionDisruptiveAction;
        m_defaultActionLogData = r.m_defaultActionLogData;
        m_defaultActionMsg = r.m_defaultActionMsg;
        m_defaultActionActionsRuntimePos = r.m_defaultActionActionsRuntimePos;
        m_defaultActionActionsSetVar = r.m_defaultActionActionsSetVar;
        m_defaultActionActionsTag = r.m_defaultActionActionsTag;
        m_transformations = r.m_transformations;
        m_defaultTransformations = r.m_defaultTransformations;
        m_severity = r.m_severity;
        m_revision = r.m_revision;
        m_version = r.m_version;
        m_accuracy = r.m_accuracy;
        m_maturity = r.m_maturity;
        m_containsCaptureAction = r.m_containsCaptureAction;
        m_containsLogAction = r.m_containsLogAction;
        m_containsNoLogAction = r.m_containsNoLogAction;
        m_containsAuditLogAction = r.m_containsAuditLogAction;
        m_containsNoAuditLogAction = r.m_containsNoAuditLogAction;
        m_containsMultiMatchAction = r.m_containsMultiMatchAction;
        m_containsStaticBlockAction = r.m_containsStaticBlockAction;
        m_defaultSeverity = r.m_defaultSeverity;
        m_defaultRevision = r.m_defaultRevision;
        m_defaultVersion = r.m_defaultVersion;
        m_defaultAccuracy = r.m_defaultAccuracy;
        m_defaultMaturity = r.m_defaultMaturity;
        m_defaultContainsCaptureAction = r.m_defaultContainsCaptureAction;
        m_defaultContainsLogAction = r.m_defaultContainsLogAction;
        m_defaultContainsNoLogAction = r.m_defaultContainsNoLogAction;
        m_defaultContainsAuditLogAction = r.m_defaultContainsAuditLogAction;
        m_defaultContainsNoAuditLogAction = r.m_defaultContainsNoAuditLogAction;
        m_defaultContainsMultiMatchAction = r.m_defaultContainsMultiMatchAction;
        m_defaultContainsStaticBlockAction = r.m_defaultContainsStaticBlockAction;
        m_isChained = r.m_isChained;
        return *this;
        // TODO: Verify if it is necessary to process any other copy.
    }


    virtual bool evaluate(Transaction *transaction) const override;


    void executeActionsIndependentOfChainedRuleResult(
        Transaction *trasn) const;

    void executeActionsAfterFullMatch(
        Transaction *trasn) const;

    static void executeAction(Transaction *trans,
        ActionWithExecution *a,
        bool context);

    void executeAction(Transaction *trans,
        ActionDisruptive *a,
        bool context) const;

    static void executeTransformation(
        Transaction *transaction,
        TransformationsResults *ret,
        Transformation *transformation);

    static void executeTransformation(
        Transaction *transaction,
        ModSecString &in,
        TransformationsResults *ret,
        Transformation *transformation);

    void executeTransformations(
        Transaction *transaction,
        const std::string &value,
        TransformationsResults &results) const;

    void addAction(actions::Action *a);
    void addTransformation(std::shared_ptr<actions::transformations::Transformation> t) {
        m_transformations.push_back(t);
    }
    void addDefaultAction(std::shared_ptr<actions::Action>);
    void addDefaultTransformation(std::shared_ptr<actions::transformations::Transformation> t) {
        m_defaultTransformations.push_back(t);
    }


    std::vector<actions::Action *> getActionsByName(const std::string& name,
        Transaction *t);
    bool containsTag(const std::string& name, Transaction *t) const;
    bool containsMsg(const std::string& name, Transaction *t) const;


    void clearDefaultActions() {
        m_defaultSeverity = SEVERITY_NOT_SET;
        m_defaultRevision = "";
        m_defaultVersion = "";
        m_defaultAccuracy = ACCURACY_NOT_SET;
        m_defaultMaturity = MATURITY_NOT_SET;
        m_defaultContainsCaptureAction = false;
        m_defaultContainsLogAction = false;
        m_defaultContainsNoLogAction = false;
        m_defaultContainsMultiMatchAction = false;
        m_defaultContainsStaticBlockAction = false;
        m_defaultActionLogData = nullptr;
        m_defaultActionMsg = nullptr;
        m_defaultActionActionsSetVar.clear();
        m_defaultActionActionsTag.clear();
        m_defaultActionActionsRuntimePos.clear();
        m_defaultActionDisruptiveAction = nullptr;
        m_defaultActionActionsRuntimePos.clear();
        m_defaultTransformations.clear();
    }

    Transformations getTransformation() const {
        Transformations dst;
        for (auto &a : m_defaultTransformations) {
            dst.push_back(a);
        }
        for (auto &a : m_transformations) {
            dst.push_back(a);
        }
        return dst;
    }

    TransformationsPtr getTransformationPtr() const {
        TransformationsPtr dst;
        for (auto &a : m_defaultTransformations) {
            dst.push_back(a.get());
        }
        for (auto &a : m_transformations) {
            dst.push_back(a.get());
        }
        return dst;
    }

    SetVars getSetVarsActions() const {
        SetVars dst;
        for (auto &a : m_defaultActionActionsSetVar) {
            dst.push_back(a);
        }
        for (auto &a : m_actionsSetVar) {
            dst.push_back(a);
        }
        return dst;
    }

    SetVarsPtr getSetVarsActionsPtr() const {
        SetVarsPtr dst;
        for (auto &a : m_defaultActionActionsSetVar) {
            dst.push_back(a.get());
        }
        for (auto &a : m_actionsSetVar) {
            dst.push_back(a.get());
        }
        return dst;
    }

    MatchActionsPtr getMatchActionsPtr() const {
        MatchActionsPtr dst;
        for (auto &a : m_defaultActionActionsRuntimePos) {
            dst.push_back(a.get());
        }
        for (auto &a : m_actionsRuntimePos) {
            dst.push_back(a.get());
        }
        return dst;
    }

    MatchActions getMatchActions() const {
        MatchActions dst;
        for (auto &a : m_defaultActionActionsRuntimePos) {
            dst.push_back(a);
        }
        for (auto &a : m_actionsRuntimePos) {
            dst.push_back(a);
        }
        return dst;
    }

    inline bool hasChainAction() const { return m_isChained == true; }
    inline void setHasChainAction(bool b) { m_isChained = b; }
    inline bool hasChainedParent() const { return m_chainedRuleParent != nullptr; }
    inline bool hasChainedChild() const { return m_chainedRuleChild.get() != nullptr; }

    inline void setHasCaptureAction(bool b) { m_containsCaptureAction = b; }
    inline bool hasCaptureAction() const { return m_containsCaptureAction || m_defaultContainsCaptureAction; }

    inline bool hasDisruptiveAction() const { return m_disruptiveAction != nullptr || m_defaultActionDisruptiveAction != nullptr; }
    inline void setDisruptiveAction(const std::shared_ptr<ActionDisruptive> &a) { m_disruptiveAction = a; }
    inline std::shared_ptr<ActionDisruptive> getDisruptiveAction() const { return m_disruptiveAction; }

    inline bool hasBlockAction() const { return m_containsStaticBlockAction || m_defaultContainsStaticBlockAction; }
    inline void setHasBlockAction(bool b) { m_containsStaticBlockAction = b; }

    inline void setHasMultimatchAction(bool b) { m_containsMultiMatchAction = b; }
    inline bool hasMultimatchAction() const { return m_containsMultiMatchAction || m_defaultContainsMultiMatchAction; }

    inline bool hasAuditLogAction() const { return m_containsAuditLogAction == true; }
    inline void setHasAuditLogAction(bool b) { m_containsAuditLogAction = b; }
    inline bool hasNoAuditLogAction() const { return m_containsNoAuditLogAction == true; }
    inline void setHasNoAuditLogAction(bool b) { m_containsNoAuditLogAction = b; }

    inline bool hasLogAction() const { return m_containsLogAction == true; }
    inline void setHasLogAction(bool b) { m_containsLogAction = b; }
    inline bool hasNoLogAction() const { return m_containsNoLogAction == true; }
    inline void setHasNoLogAction(bool b) { m_containsNoLogAction = b; }


    inline bool isItToBeLogged() const noexcept {
        if (m_containsNoLogAction) {
            return false;
        }

        if (m_defaultContainsNoLogAction && !m_containsLogAction) {
            return false;
        }

        return true;
    }


    inline bool isItToBeAuditLogged() const noexcept {
        if (!isItToBeLogged() && !m_containsAuditLogAction
            && !m_defaultContainsAuditLogAction) {
            return false;
        }

        if (m_containsNoAuditLogAction) {
            return false;
        }

        if (m_defaultContainsNoLogAction && !m_containsAuditLogAction) {
            return false;
        }

        return true;
    }


    inline bool hasLogDataAction() const { return m_logData != nullptr || m_defaultActionLogData != nullptr; }
    inline std::shared_ptr<actions::LogData> getLogDataAction() const { return m_logData; }
    std::string getLogData(/*const */Transaction *t) const;
    inline void setLogDataAction(const std::shared_ptr<actions::LogData> &data) { m_logData = data; }

    inline bool hasMessageAction() const { return m_msg != nullptr || m_defaultActionMsg != nullptr; }
    inline std::shared_ptr<actions::Msg> getMessageAction() const { return m_msg; }
    inline void setMessageAction(const std::shared_ptr<actions::Msg> &msg) { m_msg = msg; }
    std::string getMessage(/*const */Transaction *t) const;


    inline bool hasSeverityAction() const { return m_severity != SEVERITY_NOT_SET || m_defaultSeverity != SEVERITY_NOT_SET; }
    inline int getSeverity() const { return (m_severity != SEVERITY_NOT_SET)?m_severity:m_defaultSeverity; }
    inline void setDefaultActionSeverity(unsigned int severity) { m_defaultSeverity = severity; }
    inline void setSeverity(unsigned int severity) { m_severity = severity; }

    inline bool hasRevisionAction() const { return m_revision != ""; }
    inline const std::string getRevision() const { return m_revision; };
    inline void setRevision(const std::string &revision) { m_revision.assign(revision); }

    inline bool hasVersionAction() const { return m_version != ""; }
    inline const std::string getVersion() const { return m_version; };
    inline void setVersion(const std::string &version) { m_version.assign(version); }

    inline bool hasAccuracyAction() const { return m_accuracy != ACCURACY_NOT_SET || m_defaultAccuracy != ACCURACY_NOT_SET; }
    inline const int getAccuracy() const { return m_accuracy; }
    inline void setAccuracy(unsigned int accuracy) { m_accuracy = accuracy; }

    inline bool hasMaturityAction() const { return m_maturity != MATURITY_NOT_SET || m_defaultMaturity != MATURITY_NOT_SET; }
    inline const int getMaturity() const { return m_maturity; }
    inline void setDefaultActionMaturity(unsigned int maturity) { m_defaultMaturity = maturity; }
    inline void setMaturity(unsigned int maturity) { m_maturity = maturity; }

    inline bool hasTagAction() const { return m_actionsTag.size() > 0; }
    inline void setTags(Tags tags) {
        for (auto tag : tags) {
            m_actionsTag.push_back(tag);
        }
    }
    inline void cleanTags() {
        m_actionsTag.clear();
    }
    Tags getTagsAction() const {
        Tags dst;
        for (auto &a : m_defaultActionActionsTag) {
            dst.push_back(a);
        }
        for (auto &a : m_actionsTag) {
            dst.push_back(a);
        }
        return dst;
    }

    TagsPtr getTagsActionPtr() const {
        TagsPtr dst;
        for (auto &a : m_defaultActionActionsTag) {
            dst.push_back(a.get());
        }
        for (auto &a : m_actionsTag) {
            dst.push_back(a.get());
        }
        return dst;
    }

    inline RuleId getId() const { return m_ruleId; }
    void setId(int id) {
        m_ruleId = id;
    }

    void setChainedNext(std::unique_ptr<RuleWithActions> r) {
        m_chainedRuleChild = std::move(r);
    }

    inline RuleWithActions *getChainedNext() const {
        return m_chainedRuleChild.get();
    }

    void setChainedParent(RuleWithActions *r) {
        m_chainedRuleParent = r;
    }

    inline RuleWithActions *getChainedParent() {
        return m_chainedRuleParent;
    }

    XmlNSs getXmlNSs() const {
        XmlNSs dst;
        for (auto &a : m_XmlNSs) {
            dst.push_back(a);
        }
        return dst;
    }

    XmlNSsPtr getXmlNSsPtr() const {
        /**
         * FIXME: this is not conteplating SecRuleUpdateActionBy* yet.
         *
         */
        XmlNSsPtr dst;
        for (auto &a : m_XmlNSs) {
            dst.push_back(a.get());
        }
        
        return dst;
    }


    virtual void dump(std::stringstream &out) const override {
        out << "RuleWithActions" << std::endl;
    }

 private:
    RuleId m_ruleId;

    std::shared_ptr<RuleWithActions> m_chainedRuleChild;
    RuleWithActions *m_chainedRuleParent;

    /* actions */
    std::shared_ptr<ActionDisruptive> m_disruptiveAction;
    std::shared_ptr<actions::LogData> m_logData;
    std::shared_ptr<actions::Msg> m_msg;
    MatchActions m_actionsRuntimePos;
    SetVars m_actionsSetVar;
    Tags m_actionsTag;
    XmlNSs m_XmlNSs;

    /* actions || SecDefaultAction */
    std::shared_ptr<ActionDisruptive> m_defaultActionDisruptiveAction;
    std::shared_ptr<actions::LogData> m_defaultActionLogData;
    std::shared_ptr<actions::Msg> m_defaultActionMsg;

    MatchActions m_defaultActionActionsRuntimePos;
    SetVars m_defaultActionActionsSetVar;
    Tags m_defaultActionActionsTag;

    /* actions > transformations */
    Transformations m_transformations;

    /* actions > transformations || SecDefaultAction */
    Transformations m_defaultTransformations;


    /* || */
    /**
     * 0 - EMERGENCY: is generated from correlation of anomaly
     *                scoring data where there is an inbound
     *                attack and an outbound leakage.
     * 1 - ALERT: is generated from correlation where there is
     *                an inbound attack and an outbound application
     *                level error.
     * 2 - CRITICAL: Anomaly Score of 5. Is the highest severity
     *               level possible without correlation. It is
     *               normally generated by the web attack rules
     *               (40 level files).
     * 3 - ERROR: Error - Anomaly Score of 4. Is generated mostly
     *            from outbound leakage rules (50 level files).
     * 4 - WARNING: Anomaly Score of 3. Is generated by malicious
     *              client rules (35 level files).
     * 5 - NOTICE: Anomaly Score of 2. Is generated by the Protocol
     *             policy and anomaly files.
     * 6 - INFO
     * 7 - DEBUG
     **/
    unsigned int m_severity:3;

    std::string m_revision;
    std::string m_version;

    /**
     * 1-9 where 9 is very strong and 1 has many false positives
     */
    unsigned int m_accuracy:3;
    /**
     * 1-9 where 9 is extensively tested and 1 is a brand new experimental rule
     */
    unsigned int m_maturity:3;


    bool m_containsCaptureAction:1;
    bool m_containsLogAction:1;
    bool m_containsNoLogAction:1;
    bool m_containsAuditLogAction:1;
    bool m_containsNoAuditLogAction:1;
    bool m_containsMultiMatchAction:1;
    bool m_containsStaticBlockAction:1;

    /* || SecDefaultAction */
    unsigned int m_defaultSeverity:3;
    std::string m_defaultRevision;
    std::string m_defaultVersion;
    unsigned int m_defaultAccuracy:3;
    unsigned int m_defaultMaturity:3;
    bool m_defaultContainsCaptureAction:1;
    bool m_defaultContainsLogAction:1;
    bool m_defaultContainsNoLogAction:1;
    bool m_defaultContainsAuditLogAction:1;
    bool m_defaultContainsNoAuditLogAction:1;
    bool m_defaultContainsMultiMatchAction:1;
    bool m_defaultContainsStaticBlockAction:1;

    bool m_isChained:1;
};

}  // namespace modsecurity
#endif


#endif  // SRC_RULE_WITH_ACTIONS_H_
