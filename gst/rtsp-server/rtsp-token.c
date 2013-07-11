/* GStreamer
 * Copyright (C) 2010 Wim Taymans <wim.taymans at gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
/**
 * SECTION:rtsp-token
 * @short_description: Roles and permissions for a client
 * @see_also: #GstRTSPClient, #GstRTSPPermission, #GstRTSPAuth
 *
 * Last reviewed on 2013-07-11 (1.0.0)
 */

#include <string.h>

#include "rtsp-token.h"

typedef struct _GstRTSPTokenImpl
{
  GstRTSPToken token;

  GstStructure *structure;
} GstRTSPTokenImpl;

#define GST_RTSP_TOKEN_STRUCTURE(t)  (((GstRTSPTokenImpl *)(t))->structure)

//GST_DEBUG_CATEGORY_STATIC (rtsp_token_debug);
//#define GST_CAT_DEFAULT rtsp_token_debug

GST_DEFINE_MINI_OBJECT_TYPE (GstRTSPToken, gst_rtsp_token);

static void gst_rtsp_token_init (GstRTSPTokenImpl * token,
    GstStructure * structure);

static void
_gst_rtsp_token_free (GstRTSPToken * token)
{
  GstRTSPTokenImpl *impl = (GstRTSPTokenImpl *) token;

  gst_structure_set_parent_refcount (impl->structure, NULL);
  gst_structure_free (impl->structure);

  g_slice_free1 (sizeof (GstRTSPTokenImpl), token);
}

static GstRTSPToken *
_gst_rtsp_token_copy (GstRTSPTokenImpl * token)
{
  GstRTSPTokenImpl *copy;
  GstStructure *structure;

  structure = gst_structure_copy (token->structure);

  copy = g_slice_new0 (GstRTSPTokenImpl);
  gst_rtsp_token_init (copy, structure);

  return (GstRTSPToken *) copy;
}

static void
gst_rtsp_token_init (GstRTSPTokenImpl * token, GstStructure * structure)
{
  gst_mini_object_init (GST_MINI_OBJECT_CAST (token), 0,
      GST_TYPE_RTSP_TOKEN,
      (GstMiniObjectCopyFunction) _gst_rtsp_token_copy, NULL,
      (GstMiniObjectFreeFunction) _gst_rtsp_token_free);

  token->structure = structure;
  gst_structure_set_parent_refcount (token->structure,
      &token->token.mini_object.refcount);
}

/**
 * gst_rtsp_token_new:
 *
 * Create a new empty Authorization token.
 *
 * Returns: (transfer full): a new empty authorization token.
 */
GstRTSPToken *
gst_rtsp_token_new (void)
{
  GstRTSPTokenImpl *token;

  token = g_slice_new0 (GstRTSPTokenImpl);

  gst_rtsp_token_init (token, gst_structure_new_empty ("GstRTSPToken"));

  return (GstRTSPToken *) token;
}


/**
 * gst_rtsp_token_get_structure:
 * @token: The #GstRTSPToken.
 *
 * Access the structure of the token.
 *
 * Returns: The structure of the token. The structure is still
 * owned by the token, which means that you should not free it and
 * that the pointer becomes invalid when you free the token.
 *
 * MT safe.
 */
const GstStructure *
gst_rtsp_token_get_structure (GstRTSPToken * token)
{
  g_return_val_if_fail (GST_IS_RTSP_TOKEN (token), NULL);

  return GST_RTSP_TOKEN_STRUCTURE (token);
}

/**
 * gst_rtsp_token_writable_structure:
 * @token: The #GstRTSPToken.
 *
 * Get a writable version of the structure.
 *
 * Returns: The structure of the token. The structure is still
 * owned by the token, which means that you should not free it and
 * that the pointer becomes invalid when you free the token.
 * This function checks if @token is writable and will never return NULL.
 *
 * MT safe.
 */
GstStructure *
gst_rtsp_token_writable_structure (GstRTSPToken * token)
{
  g_return_val_if_fail (GST_IS_RTSP_TOKEN (token), NULL);
  g_return_val_if_fail (gst_mini_object_is_writable (GST_MINI_OBJECT_CAST
          (token)), NULL);

  return GST_RTSP_TOKEN_STRUCTURE (token);
}

/**
 * gst_rtsp_token_get_string:
 * @token: a #GstRTSPToken
 * @field: a field name
 *
 * Get the string value of @field in @token.
 *
 * Returns: the string value of @field in @token or NULL when @field is not
 * defined in @token.
 */
const gchar *
gst_rtsp_token_get_string (GstRTSPToken * token, const gchar * field)
{
  return gst_structure_get_string (GST_RTSP_TOKEN_STRUCTURE (token), field);
}