package com.music.domain;

public class User {
//	  `uid` int(50) NOT NULL AUTO_INCREMENT,
//	  `username` varchar(20) NOT NULL,
//	  `password` varchar(20) NOT NULL,
//	  `telephone` varchar(20) NOT NULL,
//	  `qq` varchar(30) NOT NULL,
//	  `email` varchar(30) NOT NULL,
//	  `registerDate` date NOT NULL
	private int uid;
	private String username;
	private String password;
	private String telephone;
	private String qq;
	private String email;
	private String registerDate;
	private String portrait;
	public int getUid() {
		return uid;
	}
	public void setUid(int uid) {
		this.uid = uid;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getTelephone() {
		return telephone;
	}
	public void setTelephone(String telephone) {
		this.telephone = telephone;
	}
	public String getQq() {
		return qq;
	}
	public void setQq(String qq) {
		this.qq = qq;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public String getRegisterDate() {
		return registerDate;
	}
	public void setRegisterDate(String registerDate) {
		this.registerDate = registerDate;
	}
	public String getPortrait() {
		return portrait;
	}
	public void setPortrait(String portrait) {
		this.portrait = portrait;
	}
	@Override
	public String toString() {
		return "User [uid=" + uid + ", username=" + username + ", password=" + password + ", telephone=" + telephone
				+ ", qq=" + qq + ", email=" + email + ", registerDate=" + registerDate + "]";
	}
}
