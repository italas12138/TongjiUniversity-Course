package com.music.domain;

public class Collection {
	private int cid;
	private String sid;
	private int uid;
	public int getCid() {
		return cid;
	}
	public void setCid(int cid) {
		this.cid = cid;
	}
	public String getSid() {
		return sid;
	}
	public void setSid(String sid) {
		this.sid = sid;
	}
	public int getUid() {
		return uid;
	}
	public void setUid(int uid) {
		this.uid = uid;
	}
	@Override
	public String toString() {
		return "Collection [cid=" + cid + ", sid=" + sid + ", uid=" + uid + "]";
	}
	
}
