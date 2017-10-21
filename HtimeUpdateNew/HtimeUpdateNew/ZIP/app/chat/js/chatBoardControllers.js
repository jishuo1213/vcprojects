/**
 * Created by chenchen on 2014/12/04
 */
'use strict';
var chatHistoryTimeOutId;

chat.controller('chatBoardCtl.contentEditCtl', ['$scope', 'contentData', '$timeout', function ($scope, contentData, $timeout) {
    $scope.friendItem = {};
    $scope.$on('fromBoard-toInput', function (d, data) {
        if (angular.isDefined($scope.toSendContent)) {
            $scope.toSendContent = $scope.toSendContent + data;
        } else {
            $scope.toSendContent = data;
        }
    });


    $scope.$on('toSengMessage', function () {
        $scope.$apply(function () {
            $scope.clickSendMessageHandler();
        });
    });
    $scope.$on('toAddEnter', function () {
        $scope.$apply(function () {
            ue.execCommand('insertHtml', '<br>');
        });
    });
    $scope.$on('sendParamToChatTo', function (d, data) {
        $scope.$apply(function () {
            $scope.friendItem = data;
        });
    });

    $scope.initFileDragUploadPlugin(document.getElementById("contentInput"));

    /*
     * 是否有正在上传的*/
    $scope.isNotExistUploadingImage = function () {
        var divId = document.createElement("div");
        var content = emoticon.parseImgToExp(ue.getContent());
        divId.innerHTML =  content ;
        if (divId.getElementsByTagName("img") != null && divId.getElementsByTagName("img").length > 0) {
            var isUploaded = false;
            var le = divId.getElementsByTagName("img").length;
            for (var i = 0; i < le; i++) {
                if (divId.getElementsByTagName("img")[i].src.indexOf("http://") == -1) {
                    isUploaded = true;
                }
            }
            return !isUploaded;
        } else {
            return true;
        }
    };

    // 发送信息
    function sendMessageHandler(sendMsgParam, callback) {
        // 没有发送图片的情况
        if ($scope.isNotExistUploadingImage()) {
            $scope.sendGroupMsgCom(sendMsgParam, callback);
        } else {
            if ($scope.isExistUploadingImageFunc(sendMsgParam.msg.content)) {
                // 还有未上传成功的情况
                sendImageData.push(sendMsgParam);
            } else {
                sendMsgParam.msg.content = window.EDC_AES.encrypt(sendMsgParam.msg.content, sessionStorage.getItem("pass"));
                // 全部上传完成了的情况
                $scope.sendGroupMsgCom(sendMsgParam, callback);
            }
        }
    }

    // 发送聊天信息
    $scope.clickSendMessageHandler = function () {
        // 退出任务只能查看
        if (readonlyByEixtTask) return;
        // 离线状态 禁止发消息
        if (!$scope.chatIsOnline) return;
        // 聊天内容为空的情况
        if (angular.isUndefined(ue.getContent()) || ue.getContent() == "") {
            showXhwAlert("温馨提示", "请输入聊天内容！");
            ue.blur();
            return;
        }

        // 如果打开了选人的页面 则整理联系人数据
        if ($scope.showSelect) {
            // 整理正在聊天的值
            $scope.setNowChattingData();
        }

        var sendItems = [];
        // 更改正在聊天的flg
        angular.forEach($scope.nowChatting, function (v) {
            // 数据存在
            if (v.id && v.id != "") {
                v.flg = "";
                sendItems.push(v);
            }
        });

        // 新创建的聊天
        if (createChat && sendItems.length == 0) {
            showXhwAlert("温馨提示", "请添加联系人！");
            return;
        }
        // 数据处理
        $scope.resetAvatarData(sendItems, 6);
        // 接收信息联系人
        var sendToArr = $scope.getToArrFunc(sendItems);
        // 没有联系人的情况
        if (angular.isUndefined(sendToArr) || sendToArr.length <= 0) {
            sendToArr = [];
        }

        var planTxt;
        var showTxt;
        var hasEmotion = false ;
        if (!$scope.isNotExistImage(ue.getContent())) {
            if ($scope.isNotExistUploadingImage()) {
                var sendContent = $scope.getImagePath(false, ue.getContent());
                showTxt = ue.getContent();
                if(emoticon.isHasEmotion(sendContent,1)){
                    hasEmotion = true ;
                }
                planTxt = window.EDC_AES.encrypt(sendContent, sessionStorage.getItem("pass"));
                console.log("planTxt=2===>",planTxt);
            } else {
                showXhwAlert("温馨提示", "图片正在上传，请稍候...");
                return;
            }
        } else {
            // 换行的处理
            if ($scope.isNotExistPre(ue.getContent())) {
                showTxt = ue.getPlainTxt();
                showTxt = emoticon.parseImgToExp(showTxt);
                showTxt = $scope.replaceAll(showTxt, "white-space: nowrap;", "");
                if(emoticon.isHasEmotion(showTxt,1)){
                    hasEmotion = true ;
                }
                planTxt = window.EDC_AES.encrypt(showTxt, sessionStorage.getItem("pass"));
            } else {
                showTxt = ue.getPlainTxt();
                console.log(emoticon.parseImgToExp(showTxt));
                var content = emoticon.parseImgToExp(ue.getPlainTxt());
                if(emoticon.isHasEmotion(content,1)){
                    hasEmotion = true ;
                }
                planTxt = window.EDC_AES.encrypt(content, sessionStorage.getItem("pass"));
            }
        }

        // 发送信息参数
        var groupParam = {
            mailId: $scope.mailId,
            msg: {
                from: {
                    id: $scope.userId,
                    uid: CurrUser.uid,
                    avatar: $scope.avatar,
                    name: $scope.userName,
                    email: $scope.userId + WY_CFG.MailLastName
                },
                to: sendToArr,
                content: planTxt,
                type: 0,
                isEncrypt: true,
                hasEmotion : hasEmotion
            }
        };
        // 把自己发送的信息显示在聊天框里
        if(hasEmotion){
            showTxt = emoticon.parseExpToImage(showTxt);
        }
        var newSend = {
            user: $scope.userId,
            avatar: HTIME_CFG._avatar_rootPath + $scope.avatar,
            name: $scope.userName,
            content: showTxt,
            time: new Date().getTime(),
            to: sendToArr,
            from: {
                id: $scope.userId,
                uid: CurrUser.uid,
                name: $scope.userName,
                avatar: $scope.avatar,
                email: $scope.userId + WY_CFG.MailLastName
            },
            class: "current-user"
        };
        // 头像处理
        if ($scope.avatar == 0) {
            newSend.avatar = "img/untitled.png";
        }
        // 任务聊天需要taskId
        if (!isPureChat) {
            // 任务Id
            groupParam.taskId = parent.Calendar.WLIsOpenTaskId;
        }
        if (!$scope.compareArrNotSort($scope.nowChatting, $scope.nowChattingBak)) {
            // 去除flg
            groupParam.updateMembers = 0;
            groupParam.userId = $scope.userId;
        }
        // 发送消息
        console.info("消息数据", groupParam);
        sendMessageHandler(groupParam, function (data) {
            console.log("sendMessageHandler", data._id);
            newSend.groupId = $scope.$parent.groupId;
            newSend._id = data._id;
        });
        newSend.msgType = "commMsg";
        $scope.$emit("sendTextMessageFrom", newSend, "message");
    };

    // 选择哪种方式发送
    $scope.clickUpdateSendMethodHanler = function (type) {
        if (2 == type) {
            $scope.sendButtonType = true;
        } else {
            $scope.sendButtonType = false;
        }
        $scope.$emit('toEnterListener', $scope.sendButtonType);
    };

    // 编写新邮件
    $scope.clickTransformToMailHandler = function () {

        // 退出任务只能查看
        if (readonlyByEixtTask) return;
        // 离线状态 禁止发消息
        if (!$scope.chatIsOnline) return;
        // 新邮件传递的参数
        var toMailParam = {
            chatId: $scope.mailId,
            taskId: parent.Calendar.WLIsOpenTaskId,
            to: JSON.parse(angular.toJson($scope.nowChatting)),
            chaobao:JSON.parse(angular.toJson($scope.unParticipation))
        };
        console.info("toMailParam=====>",toMailParam);
        parent.Calendar.previewTaskMail(0, toMailParam);
    }
}]).controller('chatBoardCtl.contentCtl', ['$scope', 'contentData', 'addMemData', '$compile', function ($scope, contentData, addMemData, $compile) {
    $scope.chatContentItems = [];
    contentData.data = [];

    $scope.$on('sendTextMessageTo', function (d, data, flg) {
        data.content = data.content.replace(/(\n)/g, "<br>");
        if (flg == "changeMember") {
            var le = $scope.chatContentItems.length;
            if ($scope.chatContentItems[le - 2].borderClass.indexOf('bottomExist') != -1) {
                $scope.chatContentItems[le - 2].borderClass = $scope.chatContentItems[le - 2].borderClass.replace("bottomExist", "leftExist");
            }
            $scope.chatContentItems.splice(le - 1, 0, data);
            contentData.data.splice(le - 1, 0, data);
        } else {
            $scope.chatContentItems.push(data);
            contentData.data.push(data);
        }
        if (parent.Calendar.selectGroup) {
            if (data.content.indexOf("img") != -1) {
                parent.Calendar.selectGroup.text("[图片]");
            } else {
                parent.Calendar.selectGroup.html(data.content.replace(/(<br>)/g, "\n"));
                parent.Calendar.selectGroup.text(parent.Calendar.selectGroup.text());
            }
            if (parent.Calendar.selectGroup_time) {
                var time = parent.doDateTime(new Date(data.time));
                parent.Calendar.selectGroup_time.text(time);
            }
        }
        if ($scope.$parent.groupId == localStorage.getItem("selectWLwindowId") && parent.Calendar.selectWindowFirst) {
            parent.Calendar.selectWindowFirst = false;
            parent.moveLi($scope.$parent.groupId);
        }
        ue.execCommand('cleardoc');
        console.log("$scope.chatContentItems", $scope.chatContentItems.length);
    });

    $scope.$on('clearData', function () {
        $scope.chatContentItems = [];
    });

    $scope.$on('toContentAddClass', function (d, data) {
        $scope.addClass($scope.chatContentItems[$scope.chatContentItems.length - 1], data);
    });

    $scope.clickAttachHandler = function ($event) {
        $event.stopPropagation();
        var url = $event.target.getAttribute('_href');
        var param = {
            fileName: $event.target.getAttribute('_name'),
            fileSize: $event.target.getAttribute('_size'),
            url: url
        };
        if (url) {
            // todo 暂时先过滤一下
            if (url.indexOf("172.30.10.20") != -1) {
                url.replace("172.30.10.20", serverIps.fileSystemIp)
            } else if (url.indexOf("10.110.9.161") != -1) {

            } else {
                url = $scope.serverImagePath + url;
            }
            //param.url = url + "&uid=" + CurrUser.uid;
            param.url = url;
            param.isChat = true;
            console.info("paramparamparamparam", param);
            parent.xhw.DownloadBox.setup(param);
        }
    };

    var folderUpload = new AttachmentUpload();

    $scope.$on('sendAttachMessageTo', function (d, data, param, file_uploaded, typeFlg, callBack) {
        $scope.$apply(function () {
            $scope.chatContentItems.push(data);
            contentData.data.push(data);
            //sessionStorage.setItem(chattingGroupId + 'localChatContentItems', JSON.stringify($scope.chatContentItems));
            ue.setContent('');
            if (param && file_uploaded && typeFlg) {
                if (navigator.onLine) {
                    // 上传附件
                    file_uploaded.userId = $scope.userId;
                    folderUpload.saveUploadFileRecord(file_uploaded, function (res) {
                        parent.Calendar.isUploading--;
                        // 上传完毕
                        $("#statusNum" + typeFlg).hide();
                        document.getElementById(typeFlg).href = res.download_url;
                        var saveUrl = res.download_url.replace($scope.serverImagePath, "");
                        $("#" + typeFlg).parent().html(
                            $compile(
                                "<div>" +
                                "<a  class='attachmentDownload' ng-click='clickAttachHandler($event)' _size='" + file_uploaded.size + "' _name='" + file_uploaded.name + "'  _href='" + saveUrl + "' id=" + typeFlg + " download>" + file_uploaded.name + "</a>" +
                                "　上传成功。</div>"
                            )($scope)
                        );
                        var uploadMsg = "<div>" +
                            "<a  class='attachmentDownload' ng-click='clickAttachHandler($event)' _size='" + file_uploaded.size + "' _name='" + file_uploaded.name + "' _href='" + saveUrl + "' id=" + typeFlg + " download>" + file_uploaded.name + "</a>" +
                            "　上传成功。</div>";
                        param.msg.content = window.EDC_AES.encrypt(uploadMsg, sessionStorage.getItem("pass"));

                        // 附件信息
                        param.msg.attachment = {
                            name: file_uploaded.name,
                            size: file_uploaded.size,
                            downloadUrl: saveUrl
                        };
                        param.msg.isEncrypt = true;
                        // 创建聊天的情况
                        if (createChat) {
                            var addMembers = null;
                            // 更新联系人列表
                            if ($scope.chattingItems.length > 1) {
                                addMembers = $scope.getToArrFunc($scope.chattingItems);
                            } else if ($scope.chattingItems.length == 1) {
                                addMembers = {
                                    id: $scope.chattingItems[0].id,
                                    name: $scope.chattingItems[0].name,
                                    avatar: $scope.chattingItems[0].avatar,
                                    email: $scope.chattingItems[0].id + WY_CFG.MailLastName,
                                    uid:$scope.chattingItems[0].uid
                                }
                            } else {
                                return;
                            }
                            parent.xhw.transceiver.createNewChat(
                                {
                                    creator: {
                                        id: $scope.userId,
                                        name: $scope.userName,
                                        avatar: $scope.avatar,
                                        email: $scope.userId + WY_CFG.MailLastName,
                                        uid:$scope.uid
                                    },
                                    members: addMembers,
                                    isGroup: $scope.chattingItems.length > 1 ? 1 : 0
                                },
                                function (data) {
                                    chattingGroupId = data.groupId;
                                    $scope.groupId = data.groupId;
                                    $scope.mailId = data.mailId;
                                    $scope.chattingItems = [];
                                    $scope.chatContentItems = [];
                                    contentData.data = [];
                                    param.mailId = data.mailId;
                                    $("#MailReceiverBox").find(".xhw_mail_user").remove();

                                    // 显示联系人的信息
                                    var contactsText = "";

                                    $scope.$apply(function () {
                                        // 添加数据到页面
                                        angular.forEach(data.member, function (v, k) {
                                            if (v.id != $scope.userId) {
                                                // 圈子或部门的情况
                                                contactsText = contactsText + v.name + "、";
                                                $scope.chattingItems.push(v);
                                            }
                                        });
                                        createChat = false;
                                        isSingle = false;
                                        $scope.sendGroupMsgCom(param);
                                        callBack();
                                    });
                                }
                            );
                        } else {
                            $scope.sendGroupMsgCom(param);
                            callBack();
                        }
                    })
                } else {
                    console.error("没联网啊亲");
                    callBack();
                }
            }
        });
    });

    $scope.$on('groupChattingHistory', function (d, data) {
        // 如果是发起人的情况
        if ($scope.showNote && $scope.showNotePad) {
            var noteItem = {};
            noteItem.class = "myNotePadShow";
            noteItem.id = "myNotePadShow";
            noteItem.msgType = "noteMsg";
            noteItem.showHistoryNum = false;
            if (!$scope.isInObjArr($scope.chatContentItems, noteItem.id)) {
                noteItem.showHistoryNum = data.length > 19;
                $scope.chatContentItems.push(noteItem);
                contentData.data.push(noteItem);
            }
            // 加载记事本
            parent.xhw.httpClient.getNotesByGroupId(
                {
                    userId: $scope.userId,
                    groupId: $scope.groupId
                },
                function (resData) {
                    $scope.$apply(function () {
                        // 随手记
                        $("#noteArea").val(resData.notes);
                        $scope.notePadContent = resData.notes;
                        // 公告板
                        $("#announcement").val(resData.announce);
                        $scope.announceContent = resData.announce;

                        if (angular.isDefined($scope.notePadContent) && $scope.notePadContent.length > 0) {
                            $(".myNoteIcon").addClass("myNoteIconShow");
                            $("#noteArea").css("background-image", "none");
                            $("#noteArea").css("height", "auto");
                        } else {
                            $("#noteArea").css("background-image", "url(img/picture.png)");
                            $("#noteArea").css("height", "80px");
                        }
                    })
                }
            );
        } else {
            var noteItem = {};
            noteItem.class = "myNotePadShow1";
            noteItem.id = "myNotePadShow1";
            noteItem.msgType = "moreMsg";
            noteItem.showHistoryNum = false;
            if (!$scope.isInObjArr($scope.chatContentItems, noteItem.id)) {
                noteItem.showHistoryNum = data.length > 19;
                $scope.chatContentItems.push(noteItem);
                contentData.data.push(noteItem);
            }
        }

        // 判断是否有更多消息记录
        //if (data.length >= 30) {
        //    var dataItem = {};
        //    dataItem.class = "moreHistoryShow";
        //    $scope.chatContentItems.push(dataItem);
        //}
        // 聊天内容
        $scope.chatContentItems = $scope.chatContentItems.concat(data);
        contentData.data = contentData.data.concat(data);
        // 存在聊天内容的情况
        if (data && data.length > 0) {
            var dataItem = {};
            dataItem.class = "historyLabelShow";
            dataItem.msgType = "hisLine";
            $scope.chatContentItems.push(dataItem);
            contentData.data.push(dataItem);
        }
        newContents = data.sort(function (a, b) {
            return a.time < b.time ? -1 : 1;
        })[0];
    });

    $scope.$on('ngRepeatFinished', function (ngRepeatFinishedEvent) {
        //下面是在table render完成后执行的js
        $("#boardContentArea a").click(function (event) {
            // 不存在打开的浏览大图可以打开
            if (event.currentTarget.href && event.currentTarget.className != "attachmentDownload" && (event.currentTarget.href.indexOf("http://") != -1 || event.currentTarget.href.indexOf("https://") != -1)) {
                parent.Calendar.previewFile(this.href);
                return false;//阻止链接跳转
            }
        });

        // 附件的点击事件
        $(".attachmentDownload").click(function (event) {
            $scope.clickAttachHandler(event);
        });

        //// 初期设置焦点
        //if ($scope.contacts != "请直接在此添加联系人") {
        //
        //}
        ue.focus();
    });

    $scope.$on('moreGroupChattingHistoryTo', function (d, data) {
        // 聊天记录插入的位置
        var index = 0;
        /*    if($scope.showNote && $scope.showNotePad) {
         index = 0;
         }  else {
         index = 0;
         }*/
        var le = data.length;
        var first = $scope.chatContentItems.shift();
        console.log("first====>", first);
        for (var i = le; i > 0; i--) {
            $scope.chatContentItems.splice(index, 0, data[i - 1]);
            contentData.data.splice(index, 0, data[i - 1])
        }
        $scope.chatContentItems.unshift(first);
        newContents = data.sort(function (a, b) {
            return a.time < b.time ? -1 : 1;
        })[0];
    });


    //根据type 各种情况啊
    $scope.doMsgType = function (data) {
        var flag = false;
        console.log("data", data);
        if ($scope.$parent.groupId == data.groupId) {
            switch (data.type) {
                case 1003:
                    //    关闭窗口并删除聊天列表
                    parent.removeLi(data.groupId);
                    flag = true;
                    break;
                case 1011:
                    //    撤回消息
                    $("#" + data.mid).remove();
                    flag = false;
                    break;
                case 1012:
                    // 删除聊天记录的同步
                    $("#" + data._id).remove();
                    flag = true;
                    break;
                case 1007:
                    //增加人员

                    break;
                case 1013:
                    //删减人员

                    break;
                case 1006:
                    //新建聊天

                    break;
                case 1014:
                    //已读消息

                    break;
            }

        } else {
            switch (data.type) {
                case 1003:
                    //    关闭窗口并删除聊天列表
                    parent.removeLi(data.groupId);
                    break;
            }
            flag = true;
        }
        return flag;
    };

    $scope.$watch('personData', function () {
        $scope.chatContentItems = [];
        contentData.data = [];
    });

    var mailIdBack;
    // 监听服务器信息
    window.onHasnewMSG = function (data) {
        // 监听信息失败
        if (data && data.error) {
            $scope.showErrorInfo(data.error);
            return;
        }
        if ($scope.doMsgType(data)) {
            return;
        }
        // 周计划聊天 任务Id不同
        if (isPlanChat && data.taskId != parent.Calendar.WLIsOpenTaskId) {
            return;
        }
        // 不是当前打开的任务的情况
        if (!$scope.$parent.groupId || (data.groupId != $scope.$parent.groupId && data.parentGroupId != $scope.$parent.groupId)) {
            parent.Calendar.getUnReaderMSG();
            return;
        }

        // 新收到的私聊信息
        if (data.isPrivate == 1) {
            if (data.from.id != $scope.userId) {
                $scope.$emit('showNewPrivateMsg', data);
                return;
            }
            return;
        }

        // 时间轴微聊的情况
        // 正在聊天
        if (data.isEncrypt || data.isMailMsg == 1) {
            try {
                if (data.isEncrypt == false) {
                    //return ;
                } else {
                    var c1 = window.EDC_AES.decrypt(data.content, sessionStorage.getItem("pass"));
                    data.content = c1;
                }
            } catch (e) {
                try {
                    var c2 = window.EDC_AES.decrypt(data.content, sessionStorage.getItem("!qaz2WSX#edc"));
                    data.content = c2;
                } catch (e) {
                    console.log("EDC_AES.decrypt", e);
                    data.content = data.content;
                }

            }
        }

        data.content = $scope.getImagePath(true, data.content);
        data.content = emoticon.parseExpToImage(data.content);
        // 纯聊天的情况 需要刷新列表
        if (isPureChat && parent.window.updateLastTo) {
            var text = data.content;
            if (!$scope.isNotExistImage(data.content)) {
                text = "[图片]"
            }
            if(emoticon.isHasEmotion(data.content,2)){
                text = "[表情]" ;
            }
            parent.window.updateLastTo($scope.$parent.groupId, text, data);
        }

        var newSend = {
            user: data.from.id,
            avatar: $scope.getGroupUserAvatar(data.from.avatar),
            name: data.from.name,
            content: data.content,
            time: data.sendTime,
            from: data.from,
            to: data.to,
            chatWith: data.chatWith,
            msgType: "commMsg",
            _id: data._id,
            groupId: data.groupId
        };
        // 邮件信息
        if (data.isMailMsg == 1) {
            // 如果是自己发个自己的 则过滤
            if (mailIdBack == data.mailId && $scope.isInObjArr(data.to, $scope.userId)) {
                return;
            }
            newSend.isMailMsg = data.isMailMsg;
            newSend.mailId = data.mailId;
            mailIdBack = data.mailId;
            newSend.to = data.to;
            newSend.msgType = "commMsg";
            var toArr = [];
            angular.forEach(data.to, function (v, k) {
                toArr.push(v.name);
            });
            newSend.addressee = toArr.join();
        }
        if (data.type == 1011 || data.type == 9999 || data.type == 1009) {
            newSend.msgType = "recallMsg";
            newSend.class = "recallMsg";
         /*   if(newSend.from.id == CurrUser.userId)
                newSend.content = newSend.content.replace(new RegExp(CurrUser.userName),"你");
            else
                newSend.content = newSend.content.replace(new RegExp("你"),newSend.from.name+" ");*/

        }
        newSend.content = newSend.content.replace(/(\n)/g, "<br>");

        if (data.class && data.class == "chatSummary") {

        } else {
            if (data.from.id == $scope.userId) {
                var sendTo =[] ;
                // 邮件信息
                if (data.isMailMsg == 1) {
                    newSend.class = "current-user mail-message";
                    sendTo = data.to.filter(function (n) {
                        return n.participation <= 1 || typeof n.participation === "undefined";
                    });
                } else {
                    sendTo = data.to ;
                    newSend.class = "current-user";
                }
                // 视频聊天的消息
                if (data.chatType && data.chatType == 'video') {
                    newSend.chatType = data.chatType;
                    newSend.class = newSend.class + " videoMsg";
                }
                $scope.$apply(function () {
                    /*修改草稿的情况，删除就草稿*/
                    var le = $scope.chatContentItems.length;
                    for (var i = 0; i < le; i++) {
                        if ($scope.chatContentItems[i].mailId == data.mailId) {
                            $scope.chatContentItems.splice(i, 1);
                            break;
                        }
                    }
                    $scope.chatContentItems.push(newSend);
                    contentData.data.push(newSend);

                    // for 视频聊天，如果发给自己的不去更新正在聊天（lastTo）
                    if (!data.chatType) {
                        $scope.$emit('updateNowChatting', data.to);
                    }
                });
                if(!(data.isMailMsg == 1))
                    parent.moveLi(data.groupId);
            } else {
                if (data.isMailMsg === 1) {
                    newSend.class = "other-user mail-message";
                } else {
                    newSend.class = "other-user";
                }
                // 视频聊天的消息
                if (data.chatType && data.chatType == 'video') {
                    newSend.chatType = data.chatType;
                    newSend.class = newSend.class + " videoMsg";
                }
                if (parent.Calendar.selectGroup) {
                    if (newSend.content.indexOf("img") != -1) {
                        parent.Calendar.selectGroup.text("[图片]");
                    } else {
                        parent.Calendar.selectGroup.html(newSend.content.replace(/(<br>)/g, "\n"));
                        parent.Calendar.selectGroup.text(parent.Calendar.selectGroup.text());
                    }
                    if (parent.Calendar.selectGroup_time) {
                        var time = parent.doDateTime(new Date(newSend.time));
                        parent.Calendar.selectGroup_time.text(time);
                    }
                }
                $scope.$apply(function () {
                    newSend.content = newSend.content.replace(/(\n)/g, "<br>");
                    $scope.chatContentItems.push(newSend);
                    contentData.data.push(newSend);
                    //判断是否是退出任务的消息
                    if (data.exitUser) {
                        $scope.$emit('updateExistMember', data);
                    }
                    //sessionStorage.setItem(chattingGroupId + 'localChatContentItems', JSON.stringify($scope.chatContentItems));
                });
                parent.Calendar.selectWindowFirst = false;
                if (parent.Calendar.groupId == localStorage.getItem("selectWLwindowId") && parent.Calendar.selectWindowFirst) {
                    parent.moveLi(parent.Calendar.groupId);
                } else if(data.type<3){
                    parent.moveLi(data.groupId);
                }
            }
        }
    };

    $scope.$on('clickMyFriendTo', function (d, data) {
        $scope.personData = data;
        chattingGroupId = $scope.personData.user_id;
    });

    $scope.$on('to-chatBoardFace', function (d, data) {
        $scope.$apply(function () {
            $scope.showFace = data;
        })
    });

    $scope.$on('getChatHistoryTo', function (d, data) {
        $scope.$apply(function () {
            $scope.chatContentItems = data;
            contentData.data = data;
            //sessionStorage.setItem(chattingGroupId + 'localChatContentItems', JSON.stringify($scope.chatContentItems));
        })
    });

    // 获取好友头像
    $scope.getFriendIcon = function (friendId) {
        var icon = "img/untitled.png";
        for (var i = 0, le = $scope.items.length; i < le; i++) {
            if (friendId == $scope.items[i].user_id) {
                icon = $scope.items[i].icon;
                break;
            }
        }
        return icon;
    };

    // 时间轴微聊获取好友头像
    $scope.getFriendAvatar = function (friendId) {
        var icon = "img/untitled.png";
        for (var i = 0, le = $scope.chattingItems.length; i < le; i++) {
            if (friendId == $scope.chattingItems[i].id) {
                icon = $scope.chattingItems[i].avatar;
                break;
            }
        }
        return icon;
    };

}]).controller('chatBoardCtl.notePadCtl', ['$scope', 'privateData', 'membersData', function ($scope, privateData, membersData) {
    //// 显示随手记的内容
    //$scope.showNoteArea = true;
    //$scope.isCreator = false;
    //if ($scope.$parent.createId == $scope.$parent.userId) {
    //    $scope.isCreator = true;
    //} else {
    //    // 显示随手记的内容
    //    $scope.showNoteArea = false;
    //}
    // 鼠标离开的情况 自动保存
    $scope.blurSaveHandler = function () {
        // 滚动条位置
        $scope.glueAuto = false;
        // 改变随手记的样式
        $scope.notePadContent = $("#noteArea").val();
        if (angular.isDefined($scope.notePadContent) && $scope.notePadContent.length > 0) {
            $(".myNoteIcon").addClass("myNoteIconShow");
        }

        parent.xhw.httpClient.setNotesByGroupId(
            {
                userId: $scope.$parent.userId,
                groupId: $scope.$parent.groupId,
                notes: $("#noteArea").val()
            },
            function (data) {
                if (angular.isUndefined($scope.notePadContent) || $scope.notePadContent.length == 0) {
                    $("#noteArea").css("background-image", "url(img/picture.png)");
                } else {
                    $("#noteArea").css("background-image", "none");
                    $("#noteArea").css("height", "auto");
                }
            }
        );
    };

    // 鼠标进入随手记
    $scope.focusNoteHandler = function () {
        $(".myNoteIcon").removeClass("myNoteIconShow");
        $("#noteArea").css("background-image", "none");
        $("#noteArea").css("height", "80px");
    };

    // 获取更多聊天记录
    $scope.showHistoryHandler = function () {
        if (!newContents) {
            $(".showMoreHistory1").hide();
            return;
        }
        var hisParam = {
            userId: $scope.$parent.userId,
            timePoint: newContents.time - 1,
            groupId: $scope.$parent.groupId,
            pageSize: 20
        };
        parent.xhw.httpClient.getGroupChatHistory(hisParam, function (data) {
            var chatHistory = data.data ;
            if (chatHistory.length == 0 || (chatHistory.length == 1 && newContents && newContents.time == chatHistory[0].sendTime)) {
                $(".showMoreHistory1").hide();
                return;
            } else if (chatHistory.length < 20) {
                $(".showMoreHistory1").hide();
            }
            $scope.$apply(function () {
                var chatHistoryItems = [];
                for (var i = 0, le = chatHistory.length; i < le; i++) {
                    var dataItem = {};
                    if (chatHistory[i].from.id == $scope.$parent.userId) {
                        dataItem.class = "current-user";
                    } else {
                        dataItem.class = "other-user";
                    }
                    if (chatHistory[i].from.avatar == 0) {
                        dataItem.avatar = "img/untitled.png";
                    } else {
                        dataItem.avatar = HTIME_CFG._avatar_rootPath + chatHistory[i].from.avatar;
                    }
                    dataItem.name = chatHistory[i].from.name;
                    dataItem.from = chatHistory[i].from;
                    dataItem.time = chatHistory[i].sendTime;
                    dataItem.createTime = chatHistory[i].createTime;
                    dataItem.content = chatHistory[i].content;
                    // 邮件信息
                    if (chatHistory[i].isMailMsg == 1) {
                        dataItem.isMailMsg = chatHistory[i].isMailMsg;
                        dataItem.mailId = chatHistory[i].mailId;
                        var toArr = [];
                        angular.forEach(chatHistory[i].to, function (v, k) {
                            toArr.push(v.name);
                        });
                        dataItem.to = toArr.join();
                        try {
                            var c1 = window.EDC_AES.decrypt(chatHistory[i].content, sessionStorage.getItem("pass"));
                            dataItem.content = c1;
                        } catch (e) {
                            try {
                                var c = window.EDC_AES.decrypt(chatHistory[i].content, '!qaz2WSX#edc');
                                dataItem.content = c;
                            } catch (e) {
                                dataItem.content = chatHistory[i].content;
                            }
                        }
                        //dataItem.content = window.EDC_AES.decrypt(chatHistory[i].content,sessionStorage.getItem("pass"));
                    } else {
                        if (chatHistory[i].isEncrypt) {
                            dataItem.content = window.EDC_AES.decrypt(chatHistory[i].content, sessionStorage.getItem("pass"));
                        } else {
                            dataItem.content = chatHistory[i].content;
                        }
                    }
                    dataItem.content = $scope.$parent.getImagePath(true, dataItem.content);
                    dataItem.msgType = "commMsg";
                    if (chatHistory[i].type == 1011 || chatHistory[i].type == systemType) {
                        dataItem.msgType = "recallMsg";
                        dataItem.class = "recallMsg";
                     /*   if(dataItem.from.id == CurrUser.userId)
                            dataItem.content = dataItem.content.replace(new RegExp(CurrUser.userName),"你");
                        else
                            dataItem.content = dataItem.content.replace(new RegExp("你"),dataItem.from.name+" ");*/
                    }
                    chatHistoryItems.push(dataItem);
                }

                console.info("chatHistoryItems", chatHistoryItems);
                // 把信息更新到页面
                $scope.$emit('moreGroupChattingHistory', chatHistoryItems);
            });
        });
    };

    // 回复
    $scope.clickReplyMailHandler = function (item, $event) {
        $event.stopPropagation();
        // 回复人
        var toArr = [];
        // 回复 收件人信息
        toArr.push(JSON.parse(angular.toJson(item.from)));
        // 回复邮件参数
        var toMailParam = {
            chatId: $scope.$parent.mailId,
            taskId: parent.Calendar.WLIsOpenTaskId,
            mailId: item.mailId,
            to: toArr
        };
        console.info(toMailParam);
        // 执行回复
        parent.Calendar.previewTaskMail(1, toMailParam);
    };

    // 全部回复
    $scope.clickReplyAllMailHandler = function (item, $event) {
        $event.stopPropagation();
        var toArr = [];
        toArr = JSON.parse(angular.toJson($scope.$parent.chattingItems));
        // 不存在发件人的情况 添加发件人
        var isExistInToArr = false;
        console.info("toArr", toArr);
        for (var i = 0, le = toArr.length; i < le; i++) {
            if (item.from.id == toArr[i].id) {
                isExistInToArr = true;
                break;
            }
        }
        console.info(toArr);
        if (!isExistInToArr) {
            var newItem = {};
            newItem.id = item.from.id;
            newItem.uid = item.from.uid;
            newItem.name = item.from.name;
            newItem.avatar = item.from.avatar;
            newItem.emial = item.from.id + WY_CFG.MailLastName;
            toArr.push(item);
        }
        var toMailParam = {
            chatId: $scope.$parent.mailId,
            taskId: parent.Calendar.WLIsOpenTaskId,
            mailId: item.mailId,
            to: toArr
        };
        console.info(toMailParam);
        // 执行全部回复
        parent.Calendar.previewTaskMail(2, toMailParam);
    };

    // 转发
    $scope.clickTransformMailHandler = function (item, $event) {
        $event.stopPropagation();
        var toMailParam = {
            chatId: $scope.$parent.mailId,
            taskId: parent.Calendar.WLIsOpenTaskId,
            mailId: item.mailId
        };
        console.info(toMailParam);
        // 执行全部回复
        parent.Calendar.previewTaskMail(3, toMailParam);
    };

    // 查看邮件信息
    $scope.clickScanMailHandler = function (item, $event) {
        // 非邮件的情况
        if (item.isMailMsg != 1) {
            //$event.stopPropagation();
            //// 展示私聊窗口，并且将数据传送到私聊窗口
            //membersData.membersObject = $scope.$parent.addObjToArr(item.to, item.from);
            //$scope.$emit('showHistoryMembers', true);
        } else {
            $event.stopPropagation();
            var toMailParam = {
                chatId: $scope.$parent.mailId,
                taskId: parent.Calendar.WLIsOpenTaskId,
                mailId: item.mailId
            };
            console.info(toMailParam);
            // 默认是读邮件
            var flg = 4;
            console.log("item",item);
            // 如果是草稿的情况
            if (item.from.isDraft =="1" || item.from.isDraft == 1) {
                flg = 5;
            }
            // 执行全部回复
            parent.Calendar.previewTaskMail(flg, toMailParam);
        }
    };

    // 点击私聊
    $scope.clickPrivateChatHandler = function (item) {
        //clearTimeout(chatHistoryTimeOutId);
        //chatHistoryTimeOutId = setTimeout(function () {
        //    $scope.$apply(function () {
        //        // 展示私聊窗口，并且将数据传送到私聊窗口
        //        membersData.membersObject = $scope.$parent.addObjToArr(item.to, item.from);
        //        $scope.$emit('showHistoryMembers', true);
        //    })
        //}, 1000);
    };

    // 点击信息
    $scope.clickBubbleMsgHandler = function (item, $event) {
        $event.stopPropagation();
        // 展示私聊窗口，并且将数据传送到私聊窗口

        // 非给定权限的人 不能查看聊天人员信息
        // console.log('$scope.showAvartaDetail',$scope.$parent.showAvartaDetail);
        if($scope.$parent.showAvartaDetail){
            membersData.membersObject = $scope.$parent.addObjToArr(item.to, item.from);
            $scope.$emit('showHistoryMembers', true, true);
        }
        //// 邮件信息的情况，查看邮件
        //if (item.isMailMsg == 1){
        //    return;
        //} else if (item.class == "other-user"){
        //    console.info("点击的消息内容", item);
        //    console.info(JSON.stringify(item.to))
        //    console.info(JSON.stringify($scope.$parent.chattingItems))
        //}
        //if (!isPureChat && item.from.id == $scope.$parent.userId) {
        // 任务聊天的情况 点击的不是以上是历史记录或者随手记的情况，去判断聊天人员发生了变化
        //if (!isPureChat && (item.class == "current-user" || item.class == "other-user" || item.class == "chatSummary")) {
        //    console.info("点击的消息内容", item);
        //    // 修改界面显示
        //    $scope.$emit('updateContacts', item);
        //}
    };
    // 鼠标离开头像1s后隐藏
    $scope.mouseLeaveHandler = function () {
        clearTimeout(chatHistoryTimeOutId);
        chatHistoryTimeOutId = setTimeout(function () {
            $scope.$apply(function () {
                // 展示私聊窗口，并且将数据传送到私聊窗口
                $scope.$emit('showHistoryMembers', false, false);
            })
        }, 1000);
    };

    // 更改头像
    $scope.clickChangeAvatarHandler = function (avatar, className) {
        if (className == "other-user") {

        }
    };

    // 解析URL
    $scope.parseURL = function (url) {
        var a = document.createElement('a');
        a.href = url;
        return {
            source: url,
            protocol: a.protocol.replace(':', ''),
            host: a.hostname,
            port: a.port,
            query: a.search,
            params: (function () {
                var ret = {},
                    seg = a.search.replace(/^\?/, '').split('&'),
                    len = seg.length, i = 0, s;
                for (; i < len; i++) {
                    if (!seg[i]) {
                        continue;
                    }
                    s = seg[i].split('=');
                    ret[s[0]] = s[1];
                }
                return ret;
            })(),
            file: (a.pathname.match(/\/([^\/?#]+)$/i) || [, ''])[1],
            hash: a.hash.replace('#', ''),
            path: a.pathname.replace(/^([^\/])/, '/$1'),
            relative: (a.href.match(/tps?:\/\/[^\/]+(.+)/) || [, ''])[1],
            segments: a.pathname.replace(/^\//, '').split('/')
        };
    };

    // 展示大图
    $scope.showImageHandler = function (imgObg) {
        var images = $(".weiliao_images");
        var imagesArr = [];
        angular.forEach(images, function (v, k) {
            var imageId = $scope.parseURL(v.src).params.doc_id;
            imagesArr.push({id: imageId, name: ""});
        });
        var imgObgId = $scope.parseURL(imgObg.src).params.doc_id;
        // 不存在打开的浏览大图可以打开
        parent.Calendar.previewImage({id: imgObgId, name: ""}, WY_CFG.preview_image_Path, imagesArr);
    };

    // 点击信息
    $scope.clickMessageHandler = function (item, $event) {
        $event.stopPropagation();
        var menus = [];
        if ($event.target.tagName == "IMG" && angular.isDefined($event.target.id) && $event.target.id.substr(0, 10) == "img_index_") {
            //  $scope.showImageHandler($event.target);
            menus.push({
                name: "预览",
                item: item,
                element: $event,
                id: 1
            });
            menus.push({
                name: "另存为...",
                item: item,
                element: $event,
                id: 6
            });
        } else if ($event.target.tagName == "A") {

        } else if (item.isMailMsg == 1) {
            //$scope.clickScanMailHandler(item, $event);
            console.log("$event", $event);
            menus.push({
                name: "查看",
                item: item,
                element: $event,
                id: 2
            });
        } else {
            menus.push({
                name: "复制",
                item: item,
                element: $event,
                id: 5
            });
        }
        if (item.time) {
            var times = new Date().getTime() - item.time;
            if (times <= 2 * 60 * 1000 && item.from.id == parent.CurrUser.userId) {
                menus.push({
                    name: "撤回",
                    item: item,
                    element: $event,
                    id: 4
                });
            }
        }
        menus.push({
            name: "删除",
            item: item,
            element: $event,
            id: 3
        });

        if (item.chatType) {
            menus.push({
                name: "加入",
                item: item,
                element: $event,
                id: 7
            });
        }
        $scope.$parent.doMenus(menus, $scope, $event);
    };

    // 双击信息
    $scope.dbclickMessageHandler = function (item,$event) {
        $event.stopPropagation();
        if ($event.target.tagName == "IMG" && angular.isDefined($event.target.id) && $event.target.id.substr(0, 10) == "img_index_") {
            $scope.$parent.mouseLeaveMsgDailogHandler();
            $scope.showImageHandler($event.target);
        }
    }
}]);
