package com.music.domain;

public class SongComment {
//	  `sid` int(50) DEFAULT NULL,
//	  `username` varchar(20) DEFAULT NULL,
//	  `comtent` varchar(200) DEFAULT NULL,
//	  `praiseNum` int(20) DEFAULT NULL,
//	  `commentDate` date DEFAULT NULL
	private String sid;
	private String username;
	private String portrait;
	private String comtent;
	private int praiseNum;
	private String commentDate;
	private String uid;
	private int fabulousId;
	public String getSid() {
		return sid;
	}
	public void setSid(String sid) {
		this.sid = sid;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getPortrait() {
		return portrait;
	}
	public void setPortrait(String portrait) {
		this.portrait = portrait;
	}
	public String getComtent() {
		return comtent;
	}
	public void setComtent(String comtent) {
		this.comtent = comtent;
	}
	public int getPraiseNum() {
		return praiseNum;
	}
	public void setPraiseNum(int praiseNum) {
		this.praiseNum = praiseNum;
	}
	public String getCommentDate() {
		return commentDate;
	}
	public void setCommentDate(String commentDate) {
		this.commentDate = commentDate;
	}
	
	public String getUid() {
		return uid;
	}
	public void setUid(String uid) {
		this.uid = uid;
	}
	
	public int getFabulousId() {
		return fabulousId;
	}
	public void setFabulousId(int fabulousId) {
		this.fabulousId = fabulousId;
	}
	@Override
	public String toString() {
		return "SongComment [sid=" + sid + ", username=" + username + ", comtent=" + comtent + ", praiseNum="
				+ praiseNum + ", commentDate=" + commentDate + "]";
	}
}
