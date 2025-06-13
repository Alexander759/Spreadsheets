#pragma once

enum class RawContentType {
	EmptyCell,
	Number,
	Bool,
	String,
	Reference,
	Expression,
	Error
};